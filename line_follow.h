#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <cmath>
#include <iostream>
#include "sensor_interface.h"

///Motor speeds to use
#define SLOW_SPEED 100
#define FAST_SPEED 200
///Differential speeds for the motors when turning
#define SLOW_DIFF 25
#define FAST_DIFF 50
///PID control values
#define Kp 1.0
#define Ki 0.0
#define Kd 0.0
#define SMALL_ERROR 0.5
#define LARGE_ERROR 1.0
#define LINE_BITS 0b00001111
#define FRONT_LINE_BITS 0b00000111
#define BACK_LINE_BITS 0b00001000

using namespace std;

extern robot_link rlink;

enum facing{
	NORTH = 0,
	EAST = 90,
	SOUTH = 180,
	WEST = 270,
	ERROR = -1
};

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
bool node_to_node(int start, int finish);
bool node_to_neighbour(int start, int finish);
bool drive_to_line();
bool rotate(facing end);
facing facing_from_node_to_node(int start, int finish);
float error(int state);
float PID(int state);
int follow_line(int state, bool speed);
