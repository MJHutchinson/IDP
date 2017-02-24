#include <robot_instr.h>
#include <robot_link.h>

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

using namespace std;

extern robot_link rlink;

float error(int state);
float PID(int state);
int follow_line(int state, bool speed);
