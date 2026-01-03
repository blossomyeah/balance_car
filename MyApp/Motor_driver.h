#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

// === 参数配置 ===
// 定时器 ARR 值 (84MHz / 20kHz - 1)
#define MOTOR_PWM_ARR 7199




// 函数声明
void Motor_Init(void);
void Motor_SetOutput(float output_L, float output_R);

#endif


