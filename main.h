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
#define ROBOT_NUM 8

#define TRUCK_1_NODE 2
#define TRUCK_2_NODE 1
#define BIN_NODE 4
#define STACK_NODE 8

using namespace std;

void mission();
void blink_empty();
bool empty_truck();
bool empty_conveyor();
bool deliver_block();
void abort_robot();

void init();
void test(char c);
void electronics_test();
void debug(string s);
void debug(stringstream s);
