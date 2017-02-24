#include "sensor_interface.h"

int get_line_follower_state(){
	return rlink.request(LINE_FOLLOWER_PORT);
}

int get_tactile_state(){
	return rlink.request(TACTILE_PORT);
}

int get_colour_state(){
	return rlink.request(COLOUR_PORT);
}

int get_ir_state(){
	return rlink.request(IR_PORT);
}

