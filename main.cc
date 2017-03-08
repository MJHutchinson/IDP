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
	cout << "SET HIGH" << endl;

	while(true){
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
		electronics_test();
		delay(100);
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

