#ifndef CONTROL_H
#define CONTROL_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "pid.h"
#include "motor_driver.h"


typedef struct {
    float angle[3]; 
    float gyro[3];  
    float accel[3]; 
    float temp;   

} attitude_t;

extern int16_t now_left_speed;
extern int16_t now_right_speed;


void control_init(void);
void task(void);
#endif

