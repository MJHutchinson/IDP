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
		if(!drive_to_line())
			return false;
	}
	
	if(map[finish].has_markers){
		if(!drive_to_line())
			return false;
	}
	
	if(drive_to_line()){
		return true;
	}
	
	return false;	
}

bool drive_to_line(){
	int state;
	int start_time = global_time.read();
	int time = start_time;
	while(true){
		state = get_line_follower_state();
		
		if((state & FRONT_LINE_BITS) == 0b111){
			break;
		}
		
		follow_line(state, true);
		//increment position
		break;
	}
	return true;
}

bool rotate(facing end){
	int state;
	int start_time = global_time.read();
	int time = start_time;
	while(true){
		state = get_line_follower_state();
		
		if(state & BACK_LINE_BITS){
			break;
		} 
		
		follow_line(state, false);
		//increment position
	}
	
	int direction = (end - current_direction);	
	
	return false;
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

void set_motors(int left, int right){
	if(left == right){
		rlink.command(BOTH_MOTORS_GO_SAME, left);
	}
}
