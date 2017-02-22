#include "sensor_interface.h"

int get_line_follower_state(){
	return rlink.request(LINE_FOLLOWER_PORT);
}
