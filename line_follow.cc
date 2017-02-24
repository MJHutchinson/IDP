#include "line_follow.h"

int follow_line(int state, bool speed){
	
	float t = PID(state);
	
	switch(state & 0b00001111){
		case 0b0001:
		case 0b0010:
		case 0b0011:
		case 0b0100:
		case 0b0110:
		case 0b1001:
		case 0b1010:
		case 0b1011:
		case 0b1100:
		case 0b1110:
			if(speed){
				rlink.command(MOTOR_1_GO, FAST_SPEED + (int)(FAST_DIFF * t));
				rlink.command(MOTOR_2_GO, FAST_SPEED - (int)(FAST_DIFF * t));
				return 2;
			}else{
				rlink.command(MOTOR_1_GO, SLOW_SPEED + (int)(SLOW_DIFF * t));
				rlink.command(MOTOR_2_GO, SLOW_SPEED - (int)(SLOW_DIFF * t));
				return 1;
			}
		case 0b0000:
		case 0b0101:
		case 0b0111:
		case 0b1000:
		case 0b1101:
		case 0b1111:
		default:
			rlink.command(BOTH_MOTORS_GO_SAME, 0);
			return -1;
	}
	return 0.0;
}

float error(int state){
	switch(state & 0b00001111){
		
		case 0b0001:
			return  LARGE_ERROR; 
		case 0b0010:
			return 0.0;
		case 0b0011:
			return  SMALL_ERROR;
		case 0b0100:
			return -LARGE_ERROR;		
		case 0b0110:
			return -SMALL_ERROR;		
		case 0b1001:
			return  LARGE_ERROR;
		case 0b1010:
			return 0.0;
		case 0b1011:
			return  SMALL_ERROR;
		case 0b1100:
			return -LARGE_ERROR;
		case 0b1110:
			return -SMALL_ERROR;
		case 0b0000:
		case 0b0101:
		case 0b0111:
		case 0b1000:
		case 0b1101:
		case 0b1111:
		default:
			return 0.0;
	}
	return 0.0;
}

float integral = 0.0;
float last_error = 0.0;

float PID(int state){
	float err = error(state);
	integral += err;
	if((state & 0b00001111) == 0b00001010)
		integral = 0.0;
	float derivative = last_error - err;
	last_error = err;
	float val = Kp * err + Ki * integral + Kd * derivative;
	return val; //(val < -1.0 ? -1.0 : val) > 1.0 ? 1.0 : val;
}
