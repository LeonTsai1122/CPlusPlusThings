#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief PID 控制器結構體
 *
 * 包含 PID 控制所需的所有參數與狀態，包含：
 * - 比例、積分、微分增益 (Kp, Ki, Kd)
 * - 目標值 (setpoint)
 * - 積分歷史紀錄 (integral)
 * - 上一次的誤差 (prev_error) 用於計算微分
 * - 積分限幅 (anti-windup limit)
 * - 輸出上下限 (output min/max)
 */
typedef struct {
    float kp;             // 比例增益 (Proportional gain)
    float ki;             // 積分增益 (Integral gain)
    float kd;             // 微分增益 (Derivative gain)

    float setpoint;       // 目標值
    float integral;       // 誤差積分累積值
    float prev_error;     // 上一次的誤差值

    float int_limit;      // 積分限幅 (Anti-windup limit)
    float out_min;        // 輸出下限
    float out_max;        // 輸出上限
} PIDController;

/**
 * @brief 初始化 PID 控制器
 *
 * @param pid       PID 控制器指標
 * @param kp        比例增益
 * @param ki        積分增益
 * @param kd        微分增益
 * @param int_limit 積分限幅，避免積分飽和
 * @param out_min   輸出下限
 * @param out_max   輸出上限
 */
void pid_init(PIDController *pid, float kp, float ki, float kd,
              float int_limit, float out_min, float out_max);

/**
 * @brief 更新 PID 增益參數
 *
 * @param pid PID 控制器指標
 * @param kp  新的比例增益
 * @param ki  新的積分增益
 * @param kd  新的微分增益
 */
void pid_set_gains(PIDController *pid, float kp, float ki, float kd);

/**
 * @brief 計算 PID 輸出 (位置型)
 *
 * 根據目前的目標值與實際反饋值，計算出控制器的輸出。
 * 此函式包含積分限幅與輸出限幅保護。
 *
 * @param pid      PID 控制器指標
 * @param feedback 目前的實際反饋值
 * @param dt       時間間隔 (delta time)，避免除以零，必須大於0
 * @return float   計算後的控制輸出值
 */
float pid_compute(PIDController *pid, float feedback, float dt);

/**
 * @brief 重置 PID 內部狀態
 *
 * 將誤差積分與上一次誤差清零，用於重新啟動控制過程。
 *
 * @param pid PID 控制器指標
 */
void pid_reset(PIDController *pid);

#ifdef __cplusplus
}
#endif

#endif // PID_H
