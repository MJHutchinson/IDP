#include <robot_instr.h>
#include <robot_link.h>

#define LINE_FOLLOWER_PORT READ_PORT_0
#define TACTILE_PORT READ_PORT_1
#define COLOUR_PORT READ_PORT_2
#define IR_PORT READ_PORT_3

extern robot_link rlink;

int get_line_follower_state();
int get_tactile_state();
int get_colour_state();
int get_ir_state();
