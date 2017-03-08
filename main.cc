#include "main.h"

using namespace std;

robot_link rlink;  
stopwatch global_time;

extern node map[];

int palletes_delivered = 0;
int palletes_on_coveyor = 0;


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

	cout << node_to_node(4,2);
}

void debug(string s){
	if(DEBUG)
		cout << s << endl;
}

