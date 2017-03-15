#include "arm_control.h"

//Initial state of the arm
arm_state state = {FRONT, false, false};

//Moves arm to a particular specified position
//Pretty dumb, shouldn't be used to interact with blocks
bool move_arm(arm_state finish){

	//If rotation is needed, retract then raise arm to avoid clashes
	if(finish.direction != state.direction){
		set_extended(false);
		set_up(true);
		set_arm_facing(finish.direction);
	}
	
	//Set the height and extension to the requested state
	set_up(finish.up);
	set_extended(finish.extended);
	
	return true;
}

//Picks up a block from a particular facing
bool pick_up_block(arm_facing direction){
	//Put arm in the correct state to begin pick up - facing the right direction, retracted and down
	move_arm((arm_state){direction, false, false});
	
	//Extend the forks under the block
	set_extended(true);
	
	//Pick up the block
	set_up(true);
	
	return true;
}

//Places a block down at a particular facing
bool put_down_block(arm_facing direction){
	//Get the material of the held block
	material mat = get_material();
	//Check if the arm is holding a block
	if(mat == NONE){
		return false;
	}
	
	//Put arm in correct place to deposit block - facing the right direction, extended and up
	move_arm((arm_state){direction, true, true});
	
	//Timeout staopwatch
	stopwatch timeout;
	timeout.start();
	
	//Lower the arm
	set_up(false);
	
	//Loop til timeout
	while(timeout.read() < ARM_TIMEOUT){
		mat = get_material();
		//If no longer holding block, retract the forks
		if(mat == NONE){
			set_extended(false);
			return true;
		}
	}
	
	return false;
	
}

//Sets the direction the arm is facing
bool set_arm_facing(arm_facing end){
	//Check if arm is already facing the right way
	if(end == state.direction){
		cout << "arm facing that way already" << endl;
		return true;
	}

	//Turn to left limit
	if(end == LEFT){
		//Start turning
		set_arm_motor(-ARM_TURN_SPEED);
		//Get the tactile button state
		int tactile_state = get_tactile_state();
		//Wait til the correct tactile button is hit
		while(!(tactile_state & LEFT_TACTILE_BIT)){
			tactile_state = get_tactile_state();
		}
		//Stop the motor
		set_arm_motor(0);
		state.direction = LEFT;
		return true;
	}
	
	//Turn to right limit
	if(end == RIGHT){
		//Start turning
		set_arm_motor(ARM_TURN_SPEED);
		//Get the tactile state
		int tactile_state = get_tactile_state();
		//Wait til correct tactial button is hit
		while(!(tactile_state & RIGHT_TACTILE_BIT)){
			tactile_state = get_tactile_state();
		}
		//Stop motor
		set_arm_motor(0);
		state.direction = RIGHT;
		return true;
	}
	
	//Rotate to the front
	if(end == FRONT){
		//If on the left side
		if(state.direction == LEFT){
			//Get the tactile state
			int tactile_state = get_tactile_state();
			//If not on the left tactile button, go there
			if(!(tactile_state & LEFT_TACTILE_BIT)){
				set_arm_motor(-ARM_TURN_SPEED);
				while(!(tactile_state & LEFT_TACTILE_BIT)){
					tactile_state = get_tactile_state();
				}
			}
			//Turn for callibrated amount of time
			set_arm_motor(ARM_TURN_SPEED);
			delay(TURN_TIME);
			set_arm_motor(0);
			return true;
		}
		
		//If on the right side
		if(state.direction == RIGHT){
			//Get tactile state
			int tactile_state = get_tactile_state();
			//If not on the right tactile button, go there
			if(!(tactile_state & RIGHT_TACTILE_BIT)){
				set_arm_motor(ARM_TURN_SPEED);
				while(!(tactile_state & RIGHT_TACTILE_BIT)){
					tactile_state = get_tactile_state();
				}
			}
			//Turn for the calibrated time
			set_arm_motor(-ARM_TURN_SPEED);
			delay(TURN_TIME);
			set_arm_motor(0);
			return true;
		}
	}
	return false;
}

//Sets if the forks are extended or not
void set_extended(bool extended){
	//Set the correct piston to the correct state
	set_pneumatic_0(extended);
	state.extended = extended;
}

//Sets if the arm is in the up or down state
void set_up(bool up){
	//Set the correct piston to the correct state
	set_pneumatic_1(up);
	state.up = up;
}

//Sets the speed of the motor. Remaps a range of -127 -> 127 to 0 -> 255 for convinience
void set_arm_motor(int speed){
	//Take a -127 to +127 speed and set the motor corrispondingly
	//If speed in the 0 -> 127 range, set the motor
	if((speed > 0) & (speed < 127)){
		rlink.command(ARM_MOTOR, speed);
	//If speed in the -127 -> 0 range, set the motor speed
	}else if((speed < 0) & (speed > -127)){
		rlink.command(ARM_MOTOR, 128 - speed);
	//Else stop the motors
	}else{
		rlink.command(ARM_MOTOR, 0);	
	}
	return;
}
