#include "pid.h"

void pid_init(PIDController *pid, float kp, float ki, float kd,
              float int_limit, float out_min, float out_max) {
    if (!pid) return;

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->setpoint = 0.0f;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;

    pid->int_limit = int_limit;
    pid->out_min = out_min;
    pid->out_max = out_max;
}

void pid_set_gains(PIDController *pid, float kp, float ki, float kd) {
    if (!pid) return;

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
}

float pid_compute(PIDController *pid, float feedback, float dt) {
    if (!pid || dt <= 0.0f) return 0.0f;

    // 計算當前誤差
    float error = pid->setpoint - feedback;

    // 比例項 (Proportional term)
    float p_term = pid->kp * error;

    // 積分項 (Integral term)
    pid->integral += error * dt;

    // 積分限幅 (Anti-windup)
    if (pid->integral > pid->int_limit) {
        pid->integral = pid->int_limit;
    } else if (pid->integral < -pid->int_limit) {
        pid->integral = -pid->int_limit;
    }
    float i_term = pid->ki * pid->integral;

    // 微分項 (Derivative term)
    float d_term = pid->kd * (error - pid->prev_error) / dt;

    // 更新上一次誤差
    pid->prev_error = error;

    // 總輸出 (Total output)
    float output = p_term + i_term + d_term;

    // 輸出限幅 (Output saturation limit)
    if (output > pid->out_max) {
        output = pid->out_max;
    } else if (output < pid->out_min) {
        output = pid->out_min;
    }

    return output;
}

void pid_reset(PIDController *pid) {
    if (!pid) return;

    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}
