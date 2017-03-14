#include "arm_control.h"

arm_facing arm_direction = FRONT;
bool arm_retracted = false;
bool arm_up = false;

bool set_arm_facing(arm_facing end){
	if(end == arm_direction){
		cout << "arm facing that way already" << endl;
		return true;
	}

	if(end == LEFT){
		set_arm_motor(-ARM_TURN_SPEED);
		int tactile_state = get_tactile_state();
		while(!(tactile_state & LEFT_TACTILE_BIT)){
			tactile_state = get_tactile_state();
		}
		set_arm_motor(0);
		arm_direction = LEFT;
		return true;
	}

	if(end == RIGHT){
		set_arm_motor(ARM_TURN_SPEED);
		int tactile_state = get_tactile_state();
		while(!(tactile_state & RIGHT_TACTILE_BIT)){
			tactile_state = get_tactile_state();
		}
		set_arm_motor(0);
		arm_direction = RIGHT;
		return true;
	}

}

void set_arm_motor(int speed){
	if((speed > 0) & (speed < 127)){
		rlink.command(ARM_MOTOR, 128 + speed);
	}else if((speed < 0) & (speed > -127)){
		rlink.command(ARM_MOTOR, - speed);
	}else{
		rlink.command(ARM_MOTOR, 0);	
	}
	return;
}
