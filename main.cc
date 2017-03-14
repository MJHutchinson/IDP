#include "main.h"

using namespace std;

robot_link rlink;  
stopwatch global_time;

extern node map[];

int palletes_delivered = 0;
int palletes_on_coveyor = 0;

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

void test(char command){

	switch(command){
		//Driving commands
		case 'd':
			rotate_to_line(1);
		case 'a':
			rotate_to_line(-1);
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
			break;
		case 'e':
			break;

		//Arm commands
		case 'i':
			set_pneumatic_0(true);
			break;
		case 'k':
			set_pneumatic_0(false);
			break;
		case 'j':
			set_pneumatic_1(true);
			break;
		case 'l':
			set_pneumatic_1(false);
			break;
		case 'u':
			break;
		case 'o':
			break;

		//Misc test commands
		case 't':
			electronics_test();
			break;
		case 'r':
			set_arm_motor(50);
		case '+':
			set_pneumatic_0(true);
			set_pneumatic_1(true);
			//rlink.command(WRITE_PORT_0, 0b11111111);
			//rlink.command(WRITE_PORT_4, 0b11111111);
			break;
		case '-':
			set_pneumatic_0(false);
			set_pneumatic_1(false);
			//rlink.command(WRITE_PORT_0, 0b00000000);
			//rlink.command(WRITE_PORT_4, 0b00000000);			
			break;
		case 'h':
			cout << "w - drive to line" << endl;
			cout << "s - put motor axis on line" << endl;
			cout << "d - rotate +90" << endl;
			cout << "a - rotate -90" << endl;
			cout << "q - flash leds 2 seconds" << endl;
			cout << "e - " << endl;
			
			cout << "i - extend arm" << endl;
			cout << "k - retract arm" << endl;
			cout << "j - rotate arm +90" << endl;
			cout << "l - rotate arm -90" << endl;
			cout << "u - arm up" << endl;
			cout << "o - arm down" << endl;

			cout << "t - test electronics" << endl;
			cout << "r - turn arm motor left" << endl;
			cout << "+ - set pneumatics high" << endl;
			cout << "- - set pneumatics low" << endl;
			cout << "k - kill all ports" << endl;
			break;
	}

}

int main(){
	
	
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

	rlink.command(RAMP_TIME, 50);

	if(DEBUG)
		cout << "INNIT: Complete in " << global_time.read() << "ms" << endl;
	
	//rlink.command(MOTOR_1_GO, 200);
	//rlink.command(MOTOR_2_GO, 200);
	//rlink.command(MOTOR_3_GO, 200);
	//rlink.command(MOTOR_4_GO, 200);

	//cout << (((true) << 1)) << endl;
	//cout << (((true) << 1) >> 1) << endl;
	//cout << (((false) << 1)) << endl;
	//cout << (((false) << 1) >> 1) << endl;
	//setup_outputs();

	//set_motors(0,100);

	 //rlink.command(MOTOR_4_GO, 100);
	//drive_to_line(true);
	//set_motors(0,0);
	//delay(5000);
	//rotate(WEST);

	//node_to_node(0,4);

	while(true){
		cout << "enter command: ";
		char command;
		command = cin.get();

		test(command);
		//follow_line(get_line_follower_state(), true, false);

				
		//rlink.command(WRITE_PORT_0, 0b00000000);		
		//rlink.command(WRITE_PORT_1, 0b00000000);		
		//rlink.command(WRITE_PORT_2, 0b00000000);		
		//rlink.command(WRITE_PORT_3, 0b00000000);		
		//rlink.command(WRITE_PORT_4, 0b00000000);		
		//rlink.command(WRITE_PORT_5, 0b00000000);
		//rlink.command(WRITE_PORT_6, 0b00000000);		
		//rlink.command(WRITE_PORT_7, 0b00000000);
		//delay(200);
		//rlink.command(WRITE_PORT_0, 0b11111111);		
		//rlink.command(WRITE_PORT_1, 0b11111111);		
		//rlink.command(WRITE_PORT_2, 0b11111111);		
		//rlink.command(WRITE_PORT_3, 0b11111111);		
		//rlink.command(WRITE_PORT_4, 0b11111111);		
		//rlink.command(WRITE_PORT_5, 0b11111111);
		//rlink.command(WRITE_PORT_6, 0b11111111);		
		//rlink.command(WRITE_PORT_7, 0b11111111);
		//delay(200);
		//electronics_test();
		//delay(100);
	//	while(!((get_line_follower_state() & 0b1111)) == 0b0000){}
	//	while((get_line_follower_state() & 0b1111) == 0b0000){}
	//	while((get_line_follower_state() & 0b111) == 0b111){}
	//	drive_to_line(true);
	//	node_to_node(0,3);
	//	set_motors(0,0);
	//  follow_line(get_line_follower_state(), true, false);
	//  set_motors(125,125);
	}
	
}

void debug(string s){
	if(DEBUG)
		cout << s << endl;
}

