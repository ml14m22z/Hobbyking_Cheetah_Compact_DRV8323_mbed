#ifndef CURRENT_CONTROLLER_CONFIG_H
#define CURRENT_CONTROLLER_CONFIG_H

#define K_D .05f                     // Volts/Amp
#define K_Q .05f                     // Volts/Amp
#define K_SCALE 0.00043f            // K_loop/Loop BW (Hz)
#define KI_D 0.0255f                  // 1/samples
#define KI_Q 0.0255f                  // 1/samples
#define V_BUS 24.0f                 // Volts

#define D_INT_LIM V_BUS/(K_D*KI_D)  // Amps*samples
#define Q_INT_LIM V_BUS/(K_Q*KI_Q)  // Amps*samples

#define I_MAX 40.0f



#endif
