#include <robot_instr.h>
#include <robot_link.h>
#include <iostream>

#include "sensor_interface.h"
#include "colour.h"
#include "material.h"

#define ARM_MOTOR MOTOR_4_GO
#define ARM_TURN_SPEED 100
#define TURN_TIME 3000

using namespace std;

enum arm_facing{
	LEFT,
	FRONT,
	RIGHT
};

bool set_arm_facing(arm_facing end);
void set_arm_motor(int speed);
