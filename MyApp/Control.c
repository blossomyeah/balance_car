#include "Control.h"
#include "encoder.h"
#include "math.h"

attitude_t attitude;
Pid_t stable_pid;
Pid_t speed_pid;
Pid_t turn_pid;
Pid_t Motor_L,Motor_R;

float target_yaw = 0.0f;
float target_speed = 2.0f;
float mech_zero = 0.1f;

int16_t now_left_speed;
int16_t now_right_speed;
int16_t now_speed;

float stable_output;
float speed_output;
float turn_output;

int16_t delta = -50;
float kp_speed = 20.0f;

void control_init(void)
{
	PID_struct_init(&stable_pid, POSITION_PID, 3000, 0, 120.0f, 0.0f, 500.0f);
	PID_struct_init(&speed_pid, POSITION_PID, 1000, 50, kp_speed,0.0f,0);
	PID_struct_init(&turn_pid, POSITION_PID, 100, 0, 12.0f, 0.0f, 0.0f);
}

void task(void)
{
	if(target_speed>=10){target_speed=10;}
	else if(target_speed<=-10)target_speed = -10;
	
	static float filter_speed = 0;
	now_left_speed = -Encoder_GetSpeed_L();
	now_right_speed = -Encoder_GetSpeed_R();
	now_speed = now_right_speed;
	if(now_speed>100)now_speed = 0;
	if(fabs(now_speed-filter_speed)<30){
	filter_speed = 0.7*filter_speed + 0.3*now_speed;
	}
	else{
		filter_speed = 0;
	}
	float now_yaw = attitude.angle[2];
	float now_pitch = -attitude.angle[1];
	
	if(fabs(now_pitch)>=30.0f){
		Motor_SetOutput(0,0);
		return ;
	}
	
	PID_calc(&stable_pid, now_pitch, mech_zero);
	stable_output = stable_pid.pos_out;
	
	PID_calc(&speed_pid, filter_speed, target_speed);
	speed_output = speed_pid.pos_out;
	
	PID_calc(&turn_pid, now_yaw, target_yaw);
	turn_output = turn_pid.pos_out;
	
	float total_output = stable_output + speed_output;

	Motor_SetOutput(total_output+delta,total_output);
}