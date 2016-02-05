#include "Transforms.h"
#include "CurrentRegulator.h"
#include "Inverter.h"
#include "SVM.h"
#include "Transforms.h"
#include "PositionSensor.h"

using namespace Transforms;

CurrentRegulator::CurrentRegulator(Inverter *inverter, PositionSensor *position_sensor, float Kp, float Ki){
    _Inverter = inverter;
    PWM = new SPWM(inverter, 2.0);
    _PositionSensor = position_sensor;
    IQ_Ref = .7;
    ID_Ref = 0;
    V_Q = 0;
    V_D = 0;
    V_Alpha = 0;
    V_Beta = 0;
    I_Q = 0;
    I_D = 0;
    I_A = 0;
    I_B = 0;
    I_C = 0;
    I_Alpha = 0;
    I_Beta = 0;
    count = 0;
    _Kp = Kp;
    _Ki = Ki;
    Q_Integral = 0;
    D_Integral = 0;
    Int_Max = .9;
    DTC_Max = .95;
    //theta_elec = _PositionSensor->GetElecPosition();

    }

void CurrentRegulator::UpdateRef(float D, float Q){
    IQ_Ref = Q;
    ID_Ref = D;
    }

void CurrentRegulator::SampleCurrent(){
    _Inverter->GetCurrent(&I_A, &I_B, &I_C);
    Clarke(I_A, I_B, &I_Alpha, &I_Beta);
    Park(I_Alpha, I_Beta, theta_elec, &I_D, &I_Q);
    count += 1;
    if(count > 10000) {
        count=0;
    //    printf("I_A:  %f        I_C:  %f       I_C:  %f\n\r", I_A, I_B, I_C);
    IQ_Ref = -IQ_Ref;
        }

    DAC->DHR12R1 = (int) (I_Q*490.648f) + 2048;
    //DAC->DHR12R1 = (int) (I_Alpha*4096.0f) + 2048;
    }
    
void CurrentRegulator::Update(){
        float Q_Error = IQ_Ref - I_Q;
        float D_Error = ID_Ref - I_D;
        
        Q_Integral += Q_Error*_Ki*_Kp;
        D_Integral += D_Error*_Ki*_Kp;
        
        if (Q_Integral > Int_Max) Q_Integral = Int_Max;
        else if(D_Integral < -Int_Max) D_Integral = Int_Max;
        if (D_Integral > Int_Max) D_Integral = Int_Max;
        else if(Q_Integral < -Int_Max) Q_Integral = Int_Max;       
         
        V_Q = Q_Integral + _Kp*Q_Error;
        V_D = D_Integral + _Kp*D_Error;
    }
        
void CurrentRegulator::SetVoltage(){
    InvPark(V_D, V_Q, theta_elec, &V_Alpha, &V_Beta);
    PWM->Update_DTC(V_Alpha, V_Beta);
    }
    
    
void CurrentRegulator::Commutate(){
    theta_elec = _PositionSensor->GetElecPosition();
    SampleCurrent();
    //Run control loop
    Update();
    SetVoltage();
    }