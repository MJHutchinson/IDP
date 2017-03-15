#include "main.h"

using namespace std;

robot_link rlink;  
stopwatch global_time;

extern node map[];
extern int current_node;
extern facing current_facing;

int palletes_to_deliver = 6;
int palletes_delivered = 0;
int palletes_on_conveyor = 0;
bool truck_1_empty = false;
bool truck_2_empty = false;
int last_truck_1_pickup = -1;
int last_truck_2_pickup = -1;

int main(){
	//Start the initialisation process
	debug("INNIT: Starting global stopwatch");
	
	global_time.start();

	#ifdef __arm__
		if (!rlink.initialise ()) {               // setup for local hardware
			return -1;
		}
	#else
		if (!rlink.initialise (ROBOT_NUM)) {      // setup the link
			cout << "Cannot initialise link" << endl;
			rlink.print_errs("    ");
			return -1;
		}
	#endif
	
	init();
	
	rlink.command(RAMP_TIME, 50);

	if(DEBUG)
		cout << "INNIT: Complete in " << global_time.read() << "ms" << endl;

	while(true){
		cout << "enter command: ";
		char command;
		command = cin.get();
		char null;
		null = cin.get();
		test(command);
	}
	
}

void init(){

	rlink.command(RAMP_TIME, 50);

}

void mission(){
	if((palletes_delivered + palletes_on_conveyor) < palletes_to_deliver){
		if(current_node == TRUCK_1_NODE){
			if(truck_1_empty){
				blink_empty();
				return;
			}else{
				if(empty_truck()){
					truck_1_empty = true;
					deliver_block();
					return;
				}else{
					return;
				}
			}
			
		}else{
			if(!truck_1_empty){
				node_to_node(TRUCK_1_NODE);
				if(empty_truck()){
					truck_1_empty = true;
					deliver_block();
					return;
				}
			}else if(!truck_2_empty){
				node_to_node(TRUCK_2_NODE);
				if(empty_truck()){
					truck_2_empty = true;
					deliver_block();
					return;
				}
			}
		}		
	}else if(palletes_delivered < 6){
		node_to_node(STACK_NODE);
		if(empty_conveyor()){
			colour col = get_colour();
			switch(col){
				case WHITE:
				case GREEN:
					put_down_block(RIGHT);
					palletes_delivered += 1;
					palletes_on_conveyor -= 1;
					return;
				case RED:
					put_down_block(LEFT);
					palletes_delivered += 1;
					palletes_on_conveyor -= 1;
					return;
				case BLACK:
				case UNKNOWN:
					abort_robot();
					return;
			}
		}else{
			return;
		}
	}else{
		node_to_node(0);
		cout << "DONE!!" << endl;
	}
}

void blink_empty(){
	set_led_empty(true);
	delay(4000);
	set_led_empty(false);
	delay(4000);
	set_led_empty(true);
	delay(4000);
	set_led_empty(false);
	truck_1_empty = false;
	truck_2_empty = false;
}

bool empty_truck(){
	pick_up_block(RIGHT);
	material mat = get_material();
	if(mat != NONE){
		return true;
	}else{
		return false;
	}
}

bool empty_conveyor(){
	pick_up_block(FRONT);
	material mat = get_material();
	if(mat != NONE){
		return true;
	}else{
		return false;
	}
}

bool deliver_block(){
	colour col = get_colour();
	set_led_colour(col);
	set_led_holding(true);
	switch(col){
		case GREEN:
		case WHITE:
		case RED:
			node_to_node(TRUCK_1_NODE);
			put_down_block(FRONT);
			palletes_on_conveyor += 1;
			set_led_holding(false);
			return true;
		case BLACK:
			node_to_node(BIN_NODE);
			//Maybe need to make a new finction to prevent clashing
			put_down_block(FRONT);
			palletes_delivered += 1;
			set_led_holding(false);
			return true;
		case UNKNOWN:
			abort_robot();
			return false;
	}
	return false;
}

void abort_robot(){
	while(true){
		set_leds(0b0000);
		delay(200);
		set_leds(0b1111);
		delay(200);
	}
}

void test(char command){

	switch(command){
		//Driving commands
		case 'd':
			rotate_to_line(1);
			break;
		case 'a':
			rotate_to_line(-1);
			break;
		case 'w':
			drive_to_line(true);
			break;
		case 's':
			motor_axis_to_line();
			break;
		case 'q':
			set_leds(0b1111);
			delay(200);
			set_leds(0b0000);
			delay(200);
			set_leds(0b1111);
			delay(200);
			set_leds(0b0000);
			delay(200);
			set_leds(0b1111);
			delay(200);
			set_leds(0b0000);
			delay(200);
			set_leds(0b1111);
			delay(200);
			set_leds(0b0000);
			delay(200);
			set_leds(0b1111);
			delay(200);
			set_leds(0b0000);
			delay(200);
			set_leds(0b1111);
			break;
		case 'e':
			break;

		//Arm commands
		case 'i':
			set_extended(true);
			break;
		case 'k':
			set_extended(false);
			break;
		case 'j':
			set_arm_facing(LEFT);
			break;
		case 'l':
			set_arm_facing(RIGHT);
			break;
		case 'u':
			set_up(true);
			break;
		case 'o':
			set_up(false);
			break;

		//Basicer move controls
		//Arm commands
		case 't':
			set_motors(100,100);
			break;
		case 'g':
			set_motors(-50,-50);
			break;
		case 'f':
			set_motors(-50,50);
			break;
		case 'h':
			set_motors(50,-50);
			break;
		case 'r':
			break;
		case 'y':
			break;

		//Misc test commands
		case 'z':
			set_motors(0,0);
			set_arm_motor(0);
			break;
		case 'x':
			electronics_test();
			break;
		case 'c':
			break;
		case 'v':
			break;
		case 'b':
			break;
		case '+':
			break;
		case '-':
			break;

		//Navigate to nodes
		case '0':
			node_to_node(0);
			break;
		case '1':
			node_to_node(1);
			break;
		case '2':
			node_to_node(2);
			break;
		case '3':
			node_to_node(3);
			break;
		case '4':
			node_to_node(4);
			break;
		case '5':
			node_to_node(5);
			break;
		case '6':
			node_to_node(6);
			break;
		case '7':
			node_to_node(7);
			break;

		case 'm':
			cout << "//DRIVE CONTROLS:" << endl;
			cout << "w - drive to line" << endl;
			cout << "s - put motor axis on line" << endl;
			cout << "d - rotate +90" << endl;
			cout << "a - rotate -90" << endl;
			cout << "q - flash leds 2 seconds" << endl;
			cout << "e - " << endl;
			
			cout << "ARM CONTROLS" << endl;
			cout << "i - extend arm" << endl;
			cout << "k - retract arm" << endl;
			cout << "j - rotate arm +90" << endl;
			cout << "l - rotate arm -90" << endl;
			cout << "u - arm up" << endl;
			cout << "o - arm down" << endl;

			cout << "BASE CONTROLS" << endl;
			cout << "t - go forward" << endl;
			cout << "g - go reverse" << endl;
			cout << "f - rotate left" << endl;
			cout << "h - rotate right" << endl;
			cout << "r - " << endl;
			cout << "y - " << endl;

			cout << "z - kill motors" << endl;
			cout << "x - test electronics" << endl;
			cout << "c - " << endl;
			cout << "v - " << endl;
			cout << "b - " << endl;
			cout << "+ - " << endl;
			cout << "- - " << endl;
			cout << "k - " << endl;
			break;
	}

}

void electronics_test(){

		cout << "ELECTRONICS TEST: ";

		//int colour_state = get_colour_state();

		//cout << "Colour state: " << colour_state;

		int strain_state = get_strain_state();
		string material = "";		

		if(strain_state > 75){
			material = "Stel";		
		}else if(strain_state > 45){
			material  = "Alum";
		}else{
			material = "None";
		}

		cout << " Strain state: " << material;

		int tactile_state = get_tactile_state();

		cout << " Tactial state: " << tactile_state;

		cout << endl;

		set_outputs();
}

void debug(string s){
	if(DEBUG)
		cout << s << endl;
}

