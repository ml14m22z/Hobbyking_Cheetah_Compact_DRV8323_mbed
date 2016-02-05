#ifndef SVM_H
#define SVM_H
#include "Inverter.h"

class SVM{
public:
    virtual void Update_DTC(float V_Alpha, float V_Beta) = 0;
private:
    float _V_Bus;
protected:
    Inverter* _inverter;
    };
    
    
class SPWM: public SVM{             //Sinusoidal PWM
public: 
    SPWM(Inverter *inverter, float V_Bus);
    virtual void Update_DTC(float V_Alpha, float V_Beta);
private:
    float _V_Bus;
protected:
    Inverter* _inverter;
    };
    
    
class SVPWM: public SVM{            //SVM
public: 
     SVPWM(Inverter *inverter, float V_Bus);
     virtual void Update_DTC(float V_Alpha, float V_Beta);

private:
    float _V_Bus;
protected:
    Inverter* _inverter;
    };
    
#endif