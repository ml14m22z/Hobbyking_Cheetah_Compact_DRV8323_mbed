#ifndef MOTOR_CONFIG_H
#define MOTOR_CONFIG_H

#define R_PHASE 0.105f          //Ohms
#define L_D 0.00003f            //Henries
#define L_Q 0.00003f            //Henries
#define KT .07f                 //N-m per peak phase amp (= RMS amps/1.5)
#define NPP 21                  //Number of pole pairs
#define GR 6.0f                 //Gear ratio
#define KT_OUT 0.45f            //Effective output torque constatnt

#define WB KT/NPP               //Webers.  



#endif
