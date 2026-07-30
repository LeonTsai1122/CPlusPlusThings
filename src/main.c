#include <stdio.h>
#include "pid.h"

// 針對 ESP-IDF 環境相容性
#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

/**
 * @brief 應用程式主要進入點 (與 ESP-IDF 相容)
 */
void app_main(void) {
    printf("開始 PID 模擬測試...\n");

    PIDController pid;
    // 初始化 PID：Kp=1.5, Ki=0.1, Kd=0.05, 積分限幅=50.0, 輸出上下限=-100.0 ~ 100.0
    pid_init(&pid, 1.5f, 0.1f, 0.05f, 50.0f, -100.0f, 100.0f);

    // 設定目標位置為 90度
    pid.setpoint = 90.0f;

    float current_position = 0.0f; // 初始位置 0 度
    float dt = 0.1f;               // 假設每次迭代 0.1 秒

    // 模擬 20 次迭代
    for (int i = 1; i <= 20; i++) {
        // 計算 PID 輸出
        float control_output = pid_compute(&pid, current_position, dt);

        // 模擬受控體 (馬達) 的行為：將控制輸出轉換為位置變化
        // 簡單模型：位置增加 = 輸出 * 某個轉換係數
        current_position += control_output * 0.1f;

        printf("迭代 %2d: 目標=%.1f, 當前位置=%.2f, 輸出=%.2f\n",
               i, pid.setpoint, current_position, control_output);

#ifdef ESP_PLATFORM
        // 在 ESP-IDF 環境中，可以使用 vTaskDelay 進行延遲
        // vTaskDelay(10 / portTICK_PERIOD_MS);
#endif
    }

    printf("PID 模擬測試結束。\n");
}

// 針對 PC 環境相容性
#ifndef ESP_PLATFORM
int main() {
    app_main();
    return 0;
}
#endif
