#include <robot_instr.h>
#include <robot_link.h>

//input ports
#define LINE_FOLLOWER_PORT READ_PORT_0
#define TACTILE_PORT READ_PORT_4
#define COLOUR_PORT READ_PORT_4
#define STRAIN_PORT ADC3
//output ports
#define LED_PORT_0 WRITE_PORT_0
#define LED_PORT_1 WRITE_PORT_4
#define PNEUMATIC_PORT WRITE_PORT_0

//port 0 bits
#define LINE_FOLLOWER_BITS 0b00001111
#define LED_BITS_0 0b00110000
#define PNEUMATIC_BITS 0b11000000
//port 1 bits
#define TACTILE_BITS 0b11000011
#define LED_BITS_1 0b00001100
#define COLOUR_BITS 0b00110000

extern robot_link rlink;

int get_line_follower_state();
int get_tactile_state();
int get_colour_state();
int get_strain_state();
//void set_LEDs(int state)();
void set_pneumatic_0(bool state);
void set_pneumatic_1(bool state);
void set_outputs();
void setup_outputs();
