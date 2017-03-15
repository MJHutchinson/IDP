#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <cmath>
#include <math.h>
#include <iostream>
#include <robot_delay.h>
#include <vector>
#include <stdlib.h> 

#include "sensor_interface.h"
#include "facing.h"

///Motor speeds to use
#define SLOW_SPEED 45
#define FAST_SPEED 75
///Differential speeds for the motors when turning in line follow
#define SLOW_DIFF 25
#define FAST_DIFF 50
///On spot turn speeds
#define SPOT_TURN_SPEED 50
///Timing delays for getting off marks on driving and turning
#define DELAY 300
#define TURN_DELAY 500
#define TIMEOUT 15000
///PID control values
#define Kp 1.0
#define Ki 0.0
#define Kd 0.0
#define SMALL_ERROR 0.2
#define LARGE_ERROR 1.0
#define LINE_BITS 0b00001111
#define FRONT_LINE_BITS 0b00000111
#define SIDE_LINE_BITS 0b00000101
#define BACK_LINE_BITS 0b00001000
//Map values
#define NODES 8

using namespace std;

extern robot_link rlink;

struct point{
	double x;
	double y;
};

struct node{
	int node;
	point position;
	int connections[3];	
	bool has_markers;
};

void set_motors(int left, int right);
bool node_to_node(int finish);
bool node_to_neighbour(int start, int finish);
bool drive_to_line(bool speed);
bool reverse_to_line(bool speed);
bool motor_axis_to_line();
bool rotate(facing end);
bool rotate_to_line(int direction);
facing facing_from_node_to_node(int start, int finish);
float error(int state);
float PID(int state);
int follow_line(int state, bool speed, bool reverse);
int find_smallest(int vals[]);
vector<int> get_path(int start, int finish);
