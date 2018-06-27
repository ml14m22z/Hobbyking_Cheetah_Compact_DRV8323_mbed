#ifndef STRUCTS_H
#define STRUCTS_H

//#include "CANnucleo.h"
#include "mbed.h"
#include "FastPWM.h"


typedef struct{
    DigitalOut *enable;
    DigitalOut *led;
    FastPWM *pwm_u, *pwm_v, *pwm_w;
    } GPIOStruct;
    
typedef struct{
    
    }COMStruct;
    
typedef struct{
    int adc1_raw, adc2_raw, adc3_raw;                       // Raw ADC Values
    float i_a, i_b, i_c;                                    // Phase currents
    float v_bus;                                            // DC link voltage
    float theta_mech, theta_elec;                           // Rotor mechanical and electrical angle
    float dtheta_mech, dtheta_elec, dtheta_elec_filt;       // Rotor mechanical and electrical angular velocit
    float i_d, i_q, i_q_filt;                               // D/Q currents
    float v_d, v_q;                                         // D/Q voltages
    float dtc_u, dtc_v, dtc_w;                              // Terminal duty cycles
    float v_u, v_v, v_w;                                    // Terminal voltages
    float k_d, k_q, ki_d, ki_q, alpha;                      // Current loop gains, current reference filter coefficient
    float d_int, q_int;                                     // Current error integrals
    int adc1_offset, adc2_offset;                           // ADC offsets
    float i_d_ref, i_q_ref, i_d_ref_filt, i_q_ref_filt;     // Current references
    float did_dt, diq_dt;                                   // Current reference derivatives
    int loop_count;                                         // Degubbing counter
    int timeout;                                            // Watchdog counter
    int mode;
    int ovp_flag;                                           // Over-voltage flag
    float p_des, v_des, kp, kd, t_ff;                       // Desired position, velocity, gians, torque
    float cogging[128];
    } ControllerStruct;

typedef struct{
    float theta_m, theta_est;
    float thetadot_m, thetadot_est;
    float i_d_m, i_d_est;
    float i_q_m, i_q_est;
    float i_d_dot, i_q_dot;
    float e_d, e_q;
    float e_d_int, e_q_int;
    } ObserverStruct;
    
#endif
