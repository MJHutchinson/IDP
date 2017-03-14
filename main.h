#include <iostream>
#include <robot_instr.h>
#include <robot_link.h>
#include <stopwatch.h>
#include <robot_delay.h>
#include <string.h>

#include "sensor_interface.h"
#include "line_follow.h"
#include "arm_control.h"
#include "facing.h"
#include "colour.h"
#include "material.h"

#define DEBUG 1
#define ROBOT_NUM  8

using namespace std;

void debug(string s);
void debug(stringstream s);
