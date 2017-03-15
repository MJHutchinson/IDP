#include "line_follow.h"

//Map of the course needed to be navigated
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

//Position on map tracking variables
int current_node = 0;
facing current_direction = EAST;
//Inertial position tracking variables
point current_position_estimate = map[0].position;
float current_direction_estimate = 90.0;

//Top level navigation function. Takes robot from and node to any other node in 
//the map
bool node_to_node(int finish){

	//Check not already at node
	if(current_node == finish){
		return true;
	}
	
	//Get the path from the current node to 
	vector<int> path = get_path(current_node, finish);

	//Grab the current and next nodes from the path
	int current  = path.back();
	path.pop_back();
	int next = path.back();
	path.pop_back();

	//Go through the path navigating from node to node
	while(!path.empty()){
		//Call function to go to the next node
		node_to_neighbour(current, next);
		//Housekeep vairables
		current = next;
		next = path.back();
		path.pop_back();
		
	}
	
	return true;
}

//Get the path from one place in the map to another. Uses an implimentation of
//Dijkstra's algorithm
vector<int> get_path(int start, int finish){
	//Populate arrays with the needed parameters for Dijkstra
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

	//Set the start node
	dist[start] = 0;

	//Print out the initial state of the arrays
	for(int k = 0; k < 8; k++){
		cout << k << ": connection: " << prev[k] << " distance: " << dist[k] << " done: " << done[k] << endl;
	}

	//Loop til all the nodes are analysed
	while(!(done[0] & done[1] & done[2] & done[3] & done[4] & done[5] & done[6] & done[7])){
		//Find the closest node to the start point that hasn't bee analysed
		int closest = -1;
		float distance = 9999.9;
		for(int n = 0; n < NODES; n++){
			if(!(done[n]) & (dist[n] < distance)){
				closest = n;
				distance = dist[n];
			}
		}
		cout << "	selected node " << closest << endl;
		
		//Check a real node is found
		if(closest == -1){
			return vector<int>();
		//Check if the finish has been found
		}else if(closest == finish){
			break;		
		//Mark this node as done
		}else{
			done[closest] = true;
		}

		//Loop through the connections of the currently selected node
		for(int j = 0; j < 3; j++){
			//Check the connection isn't a dud (-1 is a filler value for the array)
			if(Q[closest].connections[j] != -1){
				int connection = Q[closest].connections[j];
				//Work out if going from this node is a faster way to get to the connection
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
	}
	
	//Print the final state of the map
	for(int k = 0; k < 8; k++){
		cout << k << ": connection: " << prev[k] << " distance: " << dist[k] << " done: " << done[k] << endl;
	}

	//Populate a vector with the path to follow
	vector<int> path;
	int current  = finish;
	while(current != -1){
		cout << current << endl;
		path.push_back(current);
		current = prev[current];
	}

	//Return the path
	return path;
}

//Takes the robot from a node to an adjacent node
bool node_to_neighbour(int start, int finish){
	//Check at the expected node
	if(start != current_node){
		cout << "Not at the expected node. At: " << current_node << " Told to go from: " << start << endl;
		return false;
	}
	cout << "Going from node " << start << " to node " << finish << endl;
	facing direction_to_neighbour = facing_from_node_to_node(start, finish);
	//Rotate to face the correct direction if needed
	if(direction_to_neighbour != current_direction){
		cout << "rotating to face direction" << endl;
		rotate(direction_to_neighbour);
	}
	//Drive to the markers of the node started at if present
	if(map[start].has_markers){
		cout << "driving to start node markers" << endl;
		if(!drive_to_line(true))
			return false;
	}
	//Drive to the markers of the node to end at if present
	if(map[finish].has_markers){
		cout << "driving to end node markers" << endl;
		if(!drive_to_line(true))
			return false;
	}
	//Drive to the final line
	if(drive_to_line(true)){
		cout << "driving to end node" << endl;
		set_motors(0,0);
		return true;
	}
	
	current_node = finish;
	
	return false;	
}
//Drives the robot til its front line sensors are aligned with a line
bool drive_to_line(bool speed){
	int state;
	//Get robot off the current line
	set_motors(speed ? FAST_SPEED : SLOW_SPEED, speed ? FAST_SPEED : SLOW_SPEED);
	delay(DELAY);
	//Follow line til junction detected
	while(!((state & FRONT_LINE_BITS) == 0b111)){
		state = get_line_follower_state();
		
		follow_line(state, speed, false);
		//increment position
		
		//impl timeout
	}
	set_motors(0,0);
	return true;
}

//Reverses robot to line-Currently needs fixing
bool reverse_to_line(bool speed){
	int state;
	//Get robot off line
	set_motors(-SLOW_SPEED, -SLOW_SPEED);
	delay(DELAY);
	//Follow line til junction detected
	while(true){
		state = get_line_follower_state();
		
		if((state & BACK_LINE_BITS)){
			set_motors(-SLOW_SPEED, -SLOW_SPEED);
		}
		
		follow_line(state, speed, true);
		//increment position
		
		//impl timeout
	}
	set_motors(0,0);
	return true;	
}

//Rotates robot to face the correct direction
bool rotate(facing end){
	//Check not already facing requested direction
	if(end == current_direction){
		cout << "Alreday facing that way";
		return true;	
	}
	//Compute the rotation direction and magnitude
	int rotation_to_do = end - current_direction;
	int rotation_direction = (rotation_to_do > 0) ? 1 : -1;
	//Limit rotation to +/- 180 degrees
	if(rotation_to_do > 180){
		rotation_direction = -rotation_direction;
		rotation_to_do = 360 - rotation_to_do;
	}
	if(rotation_to_do < -180){
		rotation_direction = -rotation_direction;
		rotation_to_do = 360 - rotation_to_do;
	}
	//Space constraints on some nodes (e.g. trucks) makes these the correct directions to be rotating
	if((current_direction == EAST) & (rotation_direction == 1)){
		rotation_direction = -1;
		rotation_to_do = 360 - rotation_to_do;
	}
	if((current_direction == WEST) & (rotation_direction == -1)){
		rotation_direction = 1;
		rotation_to_do = 360 - rotation_to_do;
	}
	//Limit rotation scope
	while(rotation_to_do > 360){
		rotation_to_do -= 360;
	}
	while(rotation_to_do < 0){
		rotation_to_do += 360;
	}

	cout << "Rotation to do " << rotation_to_do << " Rotation direction " << rotation_direction << endl;
	//Put bakc axis on line for rotation
	motor_axis_to_line();
	//Rotate in steps of 90 degrees to make the full turn
	while(rotation_to_do > 0){
		rotate_to_line(rotation_direction);
		rotation_to_do -= 90;
		cout << "rotated 90, " << rotation_to_do << " left" << endl;
	}

	set_motors(0,0);

	current_direction = end;	
	
	return true;
}

//Follows line until motor axis is aligned with the next junction
bool motor_axis_to_line(){
	
	int state = get_line_follower_state();
	//Drive until line detected at back sensor
	while(!(state & BACK_LINE_BITS)){
		state = get_line_follower_state();
	
		follow_line(state, false, false);
		//increment position
	
		//impl timeout
	}	
	
	set_motors(0,0);

	cout << "back line found" << endl;

	return true;

}

//Rotates the robot until front line sensors pick up line
bool rotate_to_line(int direction){
	//Get the sensors off the inital line with a blind rotation
	set_motors(direction * SPOT_TURN_SPEED, direction * -SPOT_TURN_SPEED);
	delay(TURN_DELAY);	

	int state = get_line_follower_state();
	//Rotate until line detected at front of robot
	while(!(state & 0b010)){
		state = get_line_follower_state();
	}
	
	set_motors(0,0);

	return true;
}

//Computes the facing between 2 nodes
facing facing_from_node_to_node(int start, int finish){
	
	//Get the x and y distance between the nodes
	float x = map[finish].position.x - map[start].position.x;
	float y = map[finish].position.y - map[start].position.y;
	
	//Compute the facing
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
	//Get the error in position/angle of the robot
	float t = PID(state);
	int direction = reverse ? -1 : 1;
	
	//Set motors based on speed
	switch(state & 0b00000111){
		case 0b001:
		case 0b010:
		case 0b011:
		case 0b100:
		case 0b110:
		case 0b111:
		case 0b000:
		case 0b101:
			if(speed){
				set_motors((FAST_SPEED - t * FAST_DIFF) * ( direction ), (FAST_SPEED + t * FAST_DIFF) * ( direction ));
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

//Return the error as a float in position/angle of the robot from the sensor state
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
//Basic PID loop - all but the proportional disabled as integral/derivative was unstable
float PID(int state){
	//Get the actual error in the sensors
	float err = error(state);
	//Increment the integral of error
	integral += err;
	//Compute derivative
	float derivative = last_error - err;
	last_error = err;
	//Compute the PID output
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
}
