#include "mbed.h"
#include "FastPWM.h"
#include "Inverter.h"

Inverter::Inverter(PinName PinA, PinName PinB, PinName PinC, PinName PinEnable, float I_Scale, float Period){
    
    _I_Scale = I_Scale;

    
    Enable = new DigitalOut(PinEnable);
    //Current_B = new AnalogIn(BSense);
    //Current_C = new AnalogIn(CSense);
    
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable the clock to GPIOA
    RCC->APB1ENR |= 0x00000001; // enable TIM2 clock
    
    GPIOC->MODER = (1 << 8); // set pin 4 to be general purpose output

    PWM_A = new FastPWM(PinA);
    PWM_B = new FastPWM(PinB);
    PWM_C = new FastPWM(PinC);

    TIM2->CR1 &= ~(TIM_CR1_CEN);
    TIM2->CR1 |= TIM_CR1_CMS;
    TIM2->CR1 |= TIM_CR1_CEN;
     
    //PWM_A->period(Period);
    
    //PWM Setup
    TIM2->PSC = 0x0; // no prescaler, timer counts up in sync with the peripheral clock
    TIM2->ARR = 0x8CA; // 
    TIM2->CCER |= TIM_CCER_CC1NP;
         
    //ISR Setup     
    NVIC->ISER[0] |= 1<< (TIM2_IRQn); // enable the TIM2 IRQ
     
    TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
    TIM2->CR1 |= TIM_CR1_ARPE; // autoreload on, 
    TIM2->EGR |= TIM_EGR_UG;   

    // ADC Setup
     RCC->APB2ENR |= RCC_APB2ENR_ADC2EN; // clock for ADC2
     RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // clock for ADC1
     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//0x0000002; // Enable clock for GPIOC
     
     ADC->CCR = 0x00000006; // Regular simultaneous mode only
     ADC1->CR2 |= ADC_CR2_ADON;//0x00000001; // ADC1 ON
     ADC1->SQR3 = 0x000000A; // use PC_0 as input
     ADC2->CR2 |= ADC_CR2_ADON;//0x00000001; // ADC1 ON
     ADC2->SQR3 = 0x0000000B; // use PC_1 as input
     GPIOC->MODER |= 0x0000000f; // PC_0, PC_1 are analog inputs 
     
    // DAC set-up
     RCC->APB1ENR |= 0x20000000; // Enable clock for DAC
     DAC->CR |= 0x00000001; // DAC control reg, both channels ON
     GPIOA->MODER |= 0x00000300; // PA04 as analog outputs   
     
     EnableInverter();
     SetDTC(0.0f, 0.0f, 0.0f);
     wait(.2);
     ZeroCurrent();
    }

void Inverter::SetDTC(float DTC_A, float DTC_B, float DTC_C){
        PWM_A->write(DTC_A);
        PWM_B->write(DTC_B);
        PWM_C->write(DTC_C);
    }

void Inverter::EnableInverter(){
    Enable->write(1);
    }

void Inverter::DisableInverter(){
    Enable->write(0);
    }

void Inverter::ZeroCurrent(){
    I_B_Offset = 0;
    I_C_Offset = 0;
    for (int i=0; i < 1000; i++){
        I_B_Offset += ADC1->DR;
        I_C_Offset += ADC2->DR;
        ADC1->CR2  |= 0x40000000; 
        }
    I_B_Offset = I_B_Offset/1000.0f;
    I_C_Offset = I_C_Offset/1000.0f;
    printf("B_Offset:  %f     C_Offset:  %f\n\r", I_B_Offset, I_C_Offset);
    }

void Inverter::GetCurrent(float *A, float *B, float *C){
    *A = I_A;
    *B = I_B;
    *C = I_C;
    }

void Inverter::SampleCurrent(void){
 //   Dbg->write(1);
    GPIOC->ODR ^= (1 << 4);
    I_B = _I_Scale*((float) (ADC1->DR) -  I_B_Offset);
    I_C = _I_Scale*((float) (ADC2->DR)-  I_C_Offset);
    I_A = -I_B - I_C;
    //DAC->DHR12R1 = ADC2->DR; 
    //DAC->DHR12R1 = TIM3->CNT>>2;//ADC2->DR; // pass ADC -> DAC, also clears EOC flag
    ADC1->CR2  |= 0x40000000; 

    //I_B = Current_B->read()*_I_Scale;
    //I_C = Current_C->read()*_I_Scale;
    GPIOC->ODR ^= (1 << 4);
 //   Dbg->write(0);
    }
    