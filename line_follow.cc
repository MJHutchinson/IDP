#include "line_follow.h"

extern stopwatch global_time;

node map[NODES] = {
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
	//copy of map - can be edited
	
	vector<int> path = get_path(start, finish);

	int current  = path.back();
	path.pop_back();
	int next = path.back();
	path.pop_back();

	while(!path.empty()){
		node_to_neighbour(current, next);
		current = next;
		next = path.back();
		path.pop_back();
	}
	
	return true;
}

vector<int> get_path(int start, int finish){
	node Q[NODES];

	float dist[NODES];
	float prev[NODES];
	bool done[NODES];
		
	cout << "initialsie arrays" << endl;
	for(int i = 0; i < NODES; i++){
		dist[i] = 9999.8;
		prev[i] = -1;
		done[i] = false;
		Q[i] = map[i];
	}
	cout << "arrays initialised" << endl;

	dist[start] = 0;

	for(int k = 0; k < 8; k++){
		cout << k << ": connection: " << prev[k] << " distance: " << dist[k] << " done: " << done[k] << endl;
	}

	while(!(done[0] & done[1] & done[2] & done[3] & done[4] & done[5] & done[6] & done[7])){
		cout << "loop start" << endl;
		int closest = -1;
		float distance = 9999.9;
		for(int n = 0; n < NODES; n++){
			if(!(done[n]) & (dist[n] < distance)){
				closest = n;
				distance = dist[n];
			}
		}
		cout << "	selected node " << closest << endl;
		
		if(closest == -1){
			return vector<int>();
		}else if(closest == finish){
			break;		
		}else{
			done[closest] = true;
		}

		for(int j = 0; j < 3; j++){
			if(Q[closest].connections[j] != -1){
				int connection = Q[closest].connections[j];
				if(!done[connection]){
					cout << "		looking at node " << closest << " to " << connection << endl;
					float new_dist = dist[closest] + sqrt(pow(Q[closest].position.x - Q[connection].position.x, 2) + pow(Q[closest].position.y - Q[connection].position.y, 2));
					cout << "		current dist: " << dist[connection] << " new dist: " << new_dist << endl;
					if(new_dist < dist[Q[closest].connections[j]]){
						cout << "		better path found" << endl;
						dist[connection] = new_dist;
						prev[connection] = closest;
					}
				}
			}
		}
		for(int k = 0; k < 8; k++){
			cout << k << ": connection: " << prev[k] << " distance: " << dist[k] << " done: " << done[k] << endl;
		}

	}

	vector<int> path;
	int current  = finish;
	while(current != -1){
		cout << current << endl;
		path.push_back(current);
		current = prev[current];
	}

	//for(int k = 0; k < 8; k++){
	//	cout << k << ": connection: " << prev[k] << " distance: " << dist[k] << " done: " << done[k] << endl;
	//}

	return path;
}

bool node_to_neighbour(int start, int finish){
	
	if(start != current_node){
		cout << "You ain't at node " << start << " you at " << current_node << " DUH" << endl;
	}
	cout << "Going from node " << start << " to node " << finish << endl;
	facing direction_to_neighbour = facing_from_node_to_node(start, finish);
	
	if(direction_to_neighbour != current_direction){
		cout << "rotating to face direction" << endl;
		rotate(direction_to_neighbour);
	}
	
	//rlink.command(BOTH_MOTORS_GO_SAME, SLOW_SPEED);
	//int time = global_time.read();
	//while((global_time.read() - time) < 20){
	//	follow_line(get_line_follower_state(), false, false);
	//}
	
	if(map[start].has_markers){
		cout << "driving to start node markers" << endl;
		if(!drive_to_line(true))
			return false;
	}
	
	if(map[finish].has_markers){
		cout << "driving to end node markers" << endl;
		if(!drive_to_line(true))
			return false;
	}
	
	if(drive_to_line(true)){
		cout << "driving to end node" << endl;
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

	set_motors(speed ? FAST_SPEED : SLOW_SPEED, speed ? FAST_SPEED : SLOW_SPEED);
	delay(DELAY);

	while(true){
		state = get_line_follower_state();
		
		if((state & SIDE_LINE_BITS) == 0b101){
			break;
		}
		
		follow_line(state, speed, false);
		//increment position
		
		if(global_time.read() - time > TIMEOUT){
			set_motors(0,0);
			return false;
		}
	}
	return true;
}

bool reverse_to_line(bool speed){
	int state;
	int start_time = global_time.read();
	int time = start_time;

	set_motors(-SLOW_SPEED, -SLOW_SPEED);
	delay(DELAY);

	set_motors(-FAST_SPEED, -FAST_SPEED);

	while(true){
		state = get_line_follower_state();
		
		if((state & BACK_LINE_BITS)){
			set_motors(-SLOW_SPEED, -SLOW_SPEED);
		}
		
		follow_line(state, speed, true);
		//increment position
		
		if(global_time.read() - time > TIMEOUT){
			set_motors(0,0);
			return false;
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
		
		//follow_line(state, false, false);
		//increment position
		
		if(global_time.read() - time > TIMEOUT){
			set_motors(0,0);
			return false;
		}
	}
	
	set_motors( rotation_direction * SPOT_TURN_SPEED , rotation_direction * -SPOT_TURN_SPEED);

	delay(TURN_DELAY);	
	
	while(true){
		state = get_line_follower_state();
		
		if((state & 0b010)){
			break;
		}
		
		if(global_time.read() - time > TIMEOUT){
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

int follow_line(int state, bool speed, bool reverse){
	
	float t = PID(state);
	int direction = reverse ? -1 : 1;
	
	switch(state & 0b00000111){
		case 0b001:
		case 0b010:
		case 0b011:
		case 0b100:
		case 0b110:
/////////////////////////////////
//temp for test- need to figure//
		case 0b111:
		case 0b000:
		case 0b101:
			if(speed){
					//cout << " Speed: fast ";
					set_motors((FAST_SPEED - t * FAST_DIFF) * ( direction ), (FAST_SPEED + t * FAST_DIFF) * ( direction ));
					//cout << "M1E: " << FAST_SPEED - t * FAST_DIFF << " M1A: " << (rlink.request(MOTOR_1) - 128) << " M3E: " << FAST_SPEED + t * FAST_DIFF << " M3A: " << rlink.request(MOTOR_3) << " E: " << t << " " << endl;
				return 2;
			}else{
					set_motors((SLOW_SPEED - t * SLOW_DIFF) * ( direction ), (SLOW_SPEED + t * SLOW_DIFF) * ( direction ));
				return 1;
			}

		default:
			set_motors(0,0);
			return -1;
	}

	return 0;
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
