#include "sensor_interface.h"
int led_state = 0b1111;
int pneumatic_state = 0b00;

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

void set_pneumatic_0(bool state){
	pneumatic_state = (-state ^ pneumatic_state) & (1 << 0);
	set_outputs();
}
void set_pneumatic_1(bool state){
	pneumatic_state = (-state ^ pneumatic_state) & (1 << 1);
	set_outputs();
}

void set_led_colour(int state){
	led_state = ((led_state & 0b1100) | state);
	set_outputs();
}

void set_led_holding(bool state){
	led_state = ((led_state & 0b1100) | (state << 2));
	set_outputs();
}

void set_led_empty(bool state){
	led_state = ((led_state & 0b1100) | (state << 2));
	set_outputs();
}

void set_leds(int state){
	led_state = state;
	set_outputs();
}

void set_outputs(){
	int led = led_state << 2;
	int pneumatic = pneumatic_state << 6;

	int port_0 = led & LED_BITS_0;
	int port_1 = ((led & LED_BITS_1) | (pneumatic & PNEUMATIC_BITS));

	rlink.command(WRITE_PORT_0, port_0);
	rlink.command(WRITE_PORT_4, port_1);

	//led_state = ~led_state;
	//p0 = ~p0;
	//p1 = ~p1;	
}

colour get_colour(){
	int colour_state = get_colour_state();
	material mat = get_material();
	colour col;

	switch(colour_state){
		case 0:	return WHITE;
		case 1:	return RED;
		case 2:	return GREEN;
		case 3:	return BLACK;
		default: return BLACK;
	}

	if((col == BLACK) & (mat = ALUMINIUM)){
		col = GREEN;
	}
	if((col == GREEN) & (mat = STEEL)){
		col = BLACK;
	}

	return col;

}

material get_material(){	
	int strain_state = get_strain_state();
	material mat;	

	if(strain_state > 75){
		mat = STEEL;	
	}else if(strain_state > 45){
		mat = ALUMINIUM;
	}else{
		mat = NONE;
	}	

	return mat;
}
