#include "line_follow.h"

extern stopwatch global_time;

node map[8] = {
	{0, {0.0, 0.0}, {1, 5,-1}, true	},
	{1, {0.5, 0.0}, {0, 3, 2}, false},
	{2, {1.0, 0.0}, {1,-1,-1}, false},
	{3, {0.5, 0.4}, {1, 4,-1}, false},
	{4, {0.5, 0.5}, {3,-1,-1}, false},
	{5, {0.0, 1.0}, {0, 6,-1}, false},
	{6, {0.5, 1.0}, {5, 7,-1}, true	},
	{7, {1.0, 1.0}, {6,-1,-1}, false}
};

point current_position_estimate = map[0].position;
float current_direction_estimate = 90.0;
int current_node = 0;
facing current_direction = EAST;

bool node_to_node(int start, int finish){
	//Dijkstra's Algorithm Implimentaion
	return true;
}
bool node_to_neighbour(int start, int finish){
	
	if(start != current_node){
		cout << "You ain't at node " << start << " you at " << current_node << " DUH" << endl;
	}
	facing direction_to_neighbour = facing_from_node_to_node(start, finish);
	
	if(direction_to_neighbour != current_direction){
		rotate(direction_to_neighbour);
	}
	
	rlink.command(BOTH_MOTORS_GO_SAME, SLOW_SPEED);
	int time = global_time.read();
	while((global_time.read() - time) < 20){
		follow_line(get_line_follower_state(), false);
	}
	
	if(map[start].has_markers){
		if(!drive_to_line(true))
			return false;
	}
	
	if(map[finish].has_markers){
		if(!drive_to_line(true))
			return false;
	}
	
	if(drive_to_line(true)){
		set_motors(0,0);
		return true;
	}
	
	current_node = finish;
	
	return false;	
}

bool drive_to_line(bool speed){
	int state;
	int start_time = global_time.read();
	int time = start_time;

	set_motors(SLOW_SPEED, SLOW_SPEED);
	delay(DELAY);

	while(true){
		state = get_line_follower_state();
		
		if((state & FRONT_LINE_BITS) == 0b111){
			break;
		}
		
		follow_line(state, speed);
		//increment position
		
		if(global_time.read() - time > 15000){
			//set_motors(0,0);
			//return false;
		}
	}
	return true;
}

bool rotate(facing end){
	if(end == current_direction){
		cout << "Alreday facing that way";
		return true;	
	}


	int state;
	int start_time = global_time.read();
	int time = start_time;
	int rotation_to_do = end - current_direction;
	//some error check needed to not crash on trucks
	int rotation_direction = (rotation_to_do > 0) ? 1 : -1;
	cout << "Rotation to do " << rotation_to_do << " Rotation direction " << rotation_direction << endl;

	while(true){
		state = get_line_follower_state();
		
		if((state & BACK_LINE_BITS)){
			set_motors(0,0);
			break;
		}
		
		follow_line(state, false);
		//increment position
		
		if(global_time.read() - time > 15000){
			//set_motors(0,0);
			//return false;
		}
	}
	
	set_motors( rotation_direction * SPOT_TURN_SPEED , rotation_direction * -SPOT_TURN_SPEED);

	delay(TURN_DELAY);	
	
	while(true){
		state = get_line_follower_state();
		
		if(state & 0b1010){
			break;
		}
		
		if(global_time.read() - time > 15000){
			//set_motors(0,0);
			//return false;
		}
	}

	current_direction = end;	
	
	return true;
}

facing facing_from_node_to_node(int start, int finish){
	
	float x = map[finish].position.x - map[start].position.x;
	float y = map[finish].position.y - map[start].position.y;
	
	if(abs(x/y) > 0.5){
		if(x > 0)
			return EAST;
		else
			return WEST;
	}else{
		if(y > 0)
			return NORTH;
		else
			return SOUTH;
	}
	
	return ERROR;
}

int follow_line(int state, bool speed){
	
	float t = PID(state);
	
	switch(state & 0b00000111){
		case 0b001:
		case 0b010:
		case 0b011:
		case 0b100:
		case 0b110:
/////////////////////////////////
//temp for test- need to figure//
		case 0b111:
			if(speed){
					set_motors(FAST_SPEED - t * FAST_DIFF, FAST_SPEED + t * FAST_DIFF);
					//cout << "M1E: " << 128 + FAST_SPEED - t * FAST_DIFF << " M1A: " << rlink.request(MOTOR_1) << " M3E: " << FAST_SPEED + t * FAST_DIFF << " M3A: " << rlink.request(MOTOR_3) << " E: " << t << " " << endl;
				return 2;
			}else{
					set_motors(SLOW_SPEED - t * SLOW_DIFF, SLOW_SPEED + t * SLOW_DIFF);
				return 1;
			}
		case 0b000:
		case 0b101:

		default:
			set_motors(0,0);
			return -1;
	}
	return 0.0;
}

float error(int state){
	switch(state & FRONT_LINE_BITS){
		
		case 0b001:
			return  LARGE_ERROR; 
		case 0b010:
			return 0.0;
		case 0b011:
			return  SMALL_ERROR;
		case 0b100:
			return -LARGE_ERROR;		
		case 0b110:
			return -SMALL_ERROR;		
		case 0b000:
		case 0b101:
		case 0b111:
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
	float derivative = last_error - err;
	last_error = err;
	float val = Kp * err + Ki * integral + Kd * derivative;
	return val; //(val < -1.0 ? -1.0 : val) > 1.0 ? 1.0 : val;
}

// assumes between -127 and +127
void set_motors(int left, int right){
	if((left > 0) & (left < 127)){
		rlink.command(MOTOR_1_GO, 128 + left);
	}else if((left < 0) & (left > -127)){
		rlink.command(MOTOR_1_GO, - left);
	}else{
		rlink.command(MOTOR_1_GO, 0);	
	}

	if((right > 0) & (right < 127)){
		rlink.command(MOTOR_3_GO, right);
	}else if((left < 0) & (right > -127)){
		rlink.command(MOTOR_3_GO, 128 - right);
	}else{
		rlink.command(MOTOR_3_GO, 0);	
	}

	return;

//////////////////////////////////////
///MOTOR_2 Bust on our box////////////
//////////////////////////////////////
	if(left == right){
		if((left > 0) & (left < 127)){
			rlink.command(BOTH_MOTORS_GO_OPPOSITE, left);
		}else if((left < 0) & (left > - 127)){
			rlink.command(BOTH_MOTORS_GO_OPPOSITE, 128 - left);
		}else{
			rlink.command(BOTH_MOTORS_GO_OPPOSITE, 0);	
		}
	}else if(left == -right){
		if((left > 0) & (left < 127)){
			rlink.command(BOTH_MOTORS_GO_SAME, left);
		}else if((left < 0) & (left > -127)){
			rlink.command(BOTH_MOTORS_GO_SAME, 128 - left);
		}else{
			rlink.command(BOTH_MOTORS_GO_SAME, 0);	
		}
	}else{
		rlink.command(MOTOR_1_GO, left);
		rlink.command(MOTOR_2_GO, right);
	}
///////////////////////////////////////
}
