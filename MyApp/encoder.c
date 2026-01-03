#include "encoder.h"
#include "gpio.h" // 包含 GPIO 定义

// === 全局变量 (用于存储脉冲数) ===
// volatile 告诉编译器不要优化，因为它们会在中断中随时改变
volatile int32_t g_count_L = 0;
volatile int32_t g_count_R = 0;

/**
 * @brief  EXTI 外部中断回调函数 (4倍频软件解码)
 * @note   所有配置为 EXTI 的引脚触发时都会进入此函数
 * @param  GPIO_Pin: 触发中断的引脚号
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 1. 直接读取 GPIOE 端口的输入数据寄存器 (IDR)
    // 比 HAL_GPIO_ReadPin 快几十倍，减少中断耗时
    uint32_t current_pe = GPIOE->IDR;

    // ================= 左轮处理 (PE8=A, PE10=B) =================
    
    // 情况1：PE8 (A相) 发生跳变 (上升沿或下降沿)
    if (GPIO_Pin == GPIO_PIN_8) 
    {
        // 提取 A相(PE8) 和 B相(PE10) 的电平 (0或1)
        uint8_t val_A = (current_pe & GPIO_PIN_8)  ? 1 : 0;
        uint8_t val_B = (current_pe & GPIO_PIN_10) ? 1 : 0;

        // 核心逻辑：A相跳变时，如果 A==B 则加，A!=B 则减 (根据实际方向调整 ++/--)
        if (val_A == val_B) {
            g_count_L++;
        } else {
            g_count_L--;
        }
    }
    // 情况2：PE10 (B相) 发生跳变
    else if (GPIO_Pin == GPIO_PIN_10)
    {
        uint8_t val_A = (current_pe & GPIO_PIN_8)  ? 1 : 0;
        uint8_t val_B = (current_pe & GPIO_PIN_10) ? 1 : 0;

        // 核心逻辑：B相跳变时，逻辑与A相相反
        if (val_A != val_B) {
            g_count_L++;
        } else {
            g_count_L--;
        }
    }

    // ================= 右轮处理 (PE12=A, PE15=B) =================
    
    // 情况3：PE12 (A相) 发生跳变
    else if (GPIO_Pin == GPIO_PIN_12)
    {
        uint8_t val_A = (current_pe & GPIO_PIN_12) ? 1 : 0;
        uint8_t val_B = (current_pe & GPIO_PIN_15) ? 1 : 0;

        // 右轮通常安装方向相反，这里的 ++/-- 逻辑建议先写成反的，测试时再调整
        if (val_A == val_B) {
            g_count_R--; 
        } else {
            g_count_R++;
        }
    }
    // 情况4：PE15 (B相) 发生跳变
    else if (GPIO_Pin == GPIO_PIN_15)
    {
        uint8_t val_A = (current_pe & GPIO_PIN_12) ? 1 : 0;
        uint8_t val_B = (current_pe & GPIO_PIN_15) ? 1 : 0;

        if (val_A != val_B) {
            g_count_R--;
        } else {
            g_count_R++;
        }
    }
}

/**
 * @brief  获取左轮速度并清零 (用于PID计算)
 * @return 单位时间内的脉冲数 (4倍频数值)
 */
int16_t Encoder_GetSpeed_L(void)
{
    int16_t speed;
    __disable_irq(); // 关中断保护，防止读写冲突
    speed = (int16_t)g_count_L;
    g_count_L = 0;
    __enable_irq();  // 开中断
    return speed;
}

/**
 * @brief  获取右轮速度并清零
 */
int16_t Encoder_GetSpeed_R(void)
{
    int16_t speed;
    __disable_irq(); 
    speed = (int16_t)g_count_R;
    g_count_R = 0;
    __enable_irq(); 
    return speed;
}