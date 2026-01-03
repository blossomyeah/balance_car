/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Stream5;
    hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

#include "string.h"
#include "Control.h"
extern uint8_t mpu6050_data_buffer[11];
extern attitude_t attitude;
/**
 * @brief  解析MPU6050串口数据
 * @param  buffer: 接收到的11字节数据数组
 * @param  data:   用于存储解算结果的结构体指针
 * @retval uint8_t: 返回当前解析的包类型(0x51, 0x52, 0x53)，如果是0则表示无效包
 */
uint8_t MPU6050_ParseData(uint8_t *buffer, attitude_t *data)
{
    // 1. 检查帧头，必须是 0x55
    if (buffer[0] != 0x55) return 0;

    // 2. 根据第二字节判断数据包类型
    switch (buffer[1])
    {
        case 0x51: // 加速度包 (Acceleration)
            // 移位操作：高位左移8位 | 低位，强制转为short(有符号16位)，再进行物理量转换
            data->accel[0] = ((short)(buffer[3] << 8 | buffer[2])) / 32768.0f * 16.0f;
            data->accel[1] = ((short)(buffer[5] << 8 | buffer[4])) / 32768.0f * 16.0f;
            data->accel[2] = ((short)(buffer[7] << 8 | buffer[6])) / 32768.0f * 16.0f;
            data->temp     = ((short)(buffer[9] << 8 | buffer[8])) / 340.0f + 36.25f;
            break;

        case 0x52: // 角速度包 (Angular Velocity)
            data->gyro[0]  = ((short)(buffer[3] << 8 | buffer[2])) / 32768.0f * 2000.0f;
            data->gyro[1]  = ((short)(buffer[5] << 8 | buffer[4])) / 32768.0f * 2000.0f;
            data->gyro[2]  = ((short)(buffer[7] << 8 | buffer[6])) / 32768.0f * 2000.0f;
            data->temp     = ((short)(buffer[9] << 8 | buffer[8])) / 340.0f + 36.25f;
            break;

        case 0x53: // 角度包 (Angle)
            data->angle[0] = ((short)(buffer[3] << 8 | buffer[2])) / 32768.0f * 180.0f;
            data->angle[1] = ((short)(buffer[5] << 8 | buffer[4])) / 32768.0f * 180.0f;
            data->angle[2] = ((short)(buffer[7] << 8 | buffer[6])) / 32768.0f * 180.0f;
            data->temp     = ((short)(buffer[9] << 8 | buffer[8])) / 340.0f + 36.25f;
            break;

        default:
            return 0; // 未知的包类型
    }

    return buffer[1]; // 返回包类型，告知调用者更新了什么数据
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{
    if(huart->Instance == USART2)
    {

						HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)mpu6050_data_buffer,11); 
						MPU6050_ParseData(mpu6050_data_buffer,&attitude);        
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef * huart)
{
    if(huart->Instance == USART2)
    {
						HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)mpu6050_data_buffer,11); 
						memset(mpu6050_data_buffer,0, 11);                                   
    }
}

uint8_t rx_temp_byte;
uint8_t rx_buffer[6];
uint8_t rx_index = 0;
uint8_t data_valid_flag = 0;
uint8_t key_d1, key_d2, key_d3, key_d4;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        if(rx_index == 0)
        {
            if(rx_temp_byte == 0xAA)
            {
                rx_buffer[0] = rx_temp_byte;
                rx_index = 1;
            }
            else
            {
                rx_index = 0;
            }
        }
        else if(rx_index > 0 && rx_index < 5)
        {
            rx_buffer[rx_index] = rx_temp_byte;
            rx_index++;
        }
        else if(rx_index == 5)
        {
            if(rx_temp_byte == 0xFF)
            {
                rx_buffer[5] = rx_temp_byte;
                data_valid_flag = 1;

                key_d1 = rx_buffer[1];
                key_d2 = rx_buffer[2];
                key_d3 = rx_buffer[3];
                key_d4 = rx_buffer[4];
            }
            rx_index = 0;
        }
        HAL_UART_Receive_IT(&huart3, &rx_temp_byte, 1);
    }
}

/* USER CODE END 1 */
