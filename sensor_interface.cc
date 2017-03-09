#include "sensor_interface.h"
int led_state = 0b1111;
bool pneumatic_state = 0b00;

int p0 = 0b00110000;
int p1 = 0b00001100;

int get_line_follower_state(){
	return (rlink.request(LINE_FOLLOWER_PORT) & LINE_FOLLOWER_BITS);
}

int get_tactile_state(){
	int state = rlink.request(TACTILE_PORT);
	return (state & TACTILE_BITS);
}

int get_colour_state(){
	int state = rlink.request(COLOUR_PORT);
	return (state & COLOUR_BITS) >> 4;
}

int get_strain_state(){
	return rlink.request(STRAIN_PORT);
}

//void set_LEDs(int state)(){
//	led_state = state & 0b1111;
//}

void set_pneumatic_0(bool state){
	pneumatic_state = (-state ^ pneumatic_state) & (1 << 0);
	set_outputs();
}
void set_pneumatic_1(bool state){
	pneumatic_state = (-state ^ pneumatic_state) & (1 << 1);
	set_outputs();
}

void set_outputs(){
	int led = led_state << 2;
	int pneumatic = pneumatic_state << 6;

	int port_0 = led & LED_BITS_0;
	int port_1 = ((led & LED_BITS_1) | (pneumatic & PNEUMATIC_BITS));

	rlink.command(WRITE_PORT_0, p0);
	rlink.command(WRITE_PORT_4, p1);

	//led_state = ~led_state;
	//p0 = ~p0;
	//p1 = ~p1;	
}

void setup_outputs(){

	rlink.command(WRITE_PORT_0, (LED_BITS_0 | PNEUMATIC_BITS));
	rlink.command(WRITE_PORT_4, LED_BITS_1);

}
