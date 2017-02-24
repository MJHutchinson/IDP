#include "main.h"

using namespace std;

robot_link rlink;  
stopwatch global_time;


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
	
	if(DEBUG)
		cout << "INNIT: Complete in " << global_time.read() << "ms" << endl;
	
	

	while(true){
		follow_line(get_line_follower_state(), false);
	}
}

void debug(string s){
	if(DEBUG)
		cout << s << endl;
}

