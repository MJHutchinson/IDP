#include <robot_instr.h>
#include <robot_link.h>
#include <iostream>
#include <robot_delay.h>
#include <stopwatch.h>

#include "sensor_interface.h"
#include "colour.h"
#include "material.h"

#define ARM_MOTOR MOTOR_4_GO
#define ARM_TURN_SPEED 126
#define TURN_TIME 3000
#define ARM_TIMEOUT 5000

using namespace std;

enum arm_facing{
	LEFT,
	FRONT,
	RIGHT
};

struct arm_state{
	arm_facing direction;
	bool extended;
	bool up;
};

bool move_arm(arm_state state);
bool set_arm_facing(arm_facing end);
bool pick_up_block(arm_facing direction);
bool put_down_block(arm_facing direction);
void set_extended(bool extended);
void set_up(bool up);
void set_arm_motor(int speed);
