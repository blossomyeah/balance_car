#include "motor_driver.h"
#include "tim.h" 

int16_t MOTOR_DEAD_ZONE = 4010 ;
// 内部限幅函数
static int16_t Motor_Limit(int16_t val) {
    if (val > MOTOR_PWM_ARR) return MOTOR_PWM_ARR;
    if (val < -MOTOR_PWM_ARR) return -MOTOR_PWM_ARR;
    return val;
}

/**
 * @brief  死区补偿与死区处理
 * @param  input: PID计算出的原始输出值
 * @return 叠加了死区电压后的 PWM 值
 */
static int16_t Motor_AddDeadZone(float input) {
    int16_t pwm_out = 0;

    if (input > 0) {
        pwm_out = (int16_t)(input + MOTOR_DEAD_ZONE);
    }
    else {
        pwm_out = (int16_t)(input - MOTOR_DEAD_ZONE);
    }

    return Motor_Limit(pwm_out);
}

/**
 * @brief  电机初始化
 */
void Motor_Init(void) {

		HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

}

/**
 * @brief  设置左右电机输出 (包含死区补偿)
 * @param  output_L: 左轮 PID 输出结果
 * @param  output_R: 右轮 PID 输出结果
 */
void Motor_SetOutput(float output_L, float output_R) {
    int16_t pwm_L = Motor_AddDeadZone(-output_L);
    int16_t pwm_R = Motor_AddDeadZone(output_R);

    if (pwm_L >= 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_L);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, -pwm_L);
    }

    if (pwm_R >= 0) {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_R);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    } else {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, -pwm_R);
    }
}


