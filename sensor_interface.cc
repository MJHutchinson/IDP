#include "sensor_interface.h"
int led_state = 0b1111;
int pneumatic_state = 0b00;

int p0 = 0b00110000;
int p1 = 0b00001100;

//Returns the state of the line follower sensors
int get_line_follower_state(){
	return (rlink.request(LINE_FOLLOWER_PORT) & LINE_FOLLOWER_BITS);
}

//Returns the state of the tactile buttons
int get_tactile_state(){
	int state = rlink.request(TACTILE_PORT);
	return (state & TACTILE_BITS);
}

//Returns the state of thr colour inputs 
int get_colour_state(){
	int state = rlink.request(COLOUR_PORT);
	return (state & COLOUR_BITS) >> 4;
}

//Reutrns the state of the starin sensor analogue port
int get_strain_state(){
	return rlink.request(STRAIN_PORT);
}

//Sets pnuematice port 0 high
void set_pneumatic_0(bool state){
	//Sets the bit in the LED state to the correct state
	if(state){
		pneumatic_state |= (1u << 0);
	}else{
		pneumatic_state &= ~(1u << 0);
	}
	set_outputs();
}

//set pneumatic port 1 high
void set_pneumatic_1(bool state){
	//Sets the bit in the LED state to the correct state
	if(state){
		pneumatic_state |= (1u << 1);
	}else{
		pneumatic_state &= ~(1u << 1);
	}
	set_outputs();
}

//Sets the LEDs indicating colour
void set_led_colour(int state){
	led_state = ((led_state & 0b1100) | state);
	set_outputs();
}

//Sets the LED to indicate arm is holding a pallete
void set_led_holding(bool state){
	led_state = ((led_state & 0b1011) | (state << 2));
	set_outputs();
}

//Sets the refil request LED
void set_led_empty(bool state){
	led_state = ((led_state & 0b0111) | (state << 3));
	set_outputs();
}

//Sets all the LEDs states
void set_leds(int state){
	led_state = state;
	set_outputs();
}

//Takes all the states of the outputs needed and sets them
void set_outputs(){
	//Shifts states into correct bit positions
	int led = led_state << 2;
	int pneumatic = pneumatic_state << 6;

	//Calculates the needed port states
	int port_0 = ((led & LED_BITS_0) | (pneumatic & PNEUMATIC_BITS));
	int port_4 = led & LED_BITS_1;

	rlink.command(WRITE_PORT_0, port_0);
	rlink.command(WRITE_PORT_4, port_4);

	//led_state = ~led_state;
	//p0 = ~p0;
	//p1 = ~p1;	
}

//Gets the colour of the block being held
colour get_colour(){
	int colour_state = get_colour_state();
	material mat = get_material();
	colour col;

	//Get the colour from sensor state
	switch(colour_state){
		case 0:	return WHITE;
		case 1:	return RED;
		case 2:	return GREEN;
		case 3:	return BLACK;
		default: return BLACK;
	}

	//Error check for LDR error
	if((col == BLACK) & (mat = ALUMINIUM)){
		col = GREEN;
	}
	if((col == GREEN) & (mat = STEEL)){
		col = BLACK;
	}

	return col;

}

//Gets the material being held by the forks
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
