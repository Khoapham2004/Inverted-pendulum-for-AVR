
#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float setpoint;
    float integral;
    float previous_error;
    float output_min;
    float output_max;
} PID_t;

void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float output_min, float output_max);
float PID_Compute(PID_t *pid, float measured_value, float dt);

void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float output_min, float output_max) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->setpoint = 0.0;
    pid->integral = 0.0;
    pid->previous_error = 0.0;
    pid->output_min = output_min;
    pid->output_max = output_max;
}

float PID_Compute(PID_t *pid, float measured_value, float dt) {
    float error = pid->setpoint - measured_value;
    pid->integral += error * dt;
    float derivative = (error - pid->previous_error) / dt;
    float output = (pid->Kp * error) + (pid->Ki * pid->integral) + (pid->Kd * derivative);
    
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }
    
    pid->previous_error = error;
    return output;
}

#endif // PID_H
