#include "Mixer.h"

#include <signal.h>
#include <sys/types.h>
#include <termios.h>

struct termios tsave;

/* sets canonical input mode */
void set_terminal() {
	struct termios tbuf;
	tcgetattr(0,&tbuf);
	tsave=tbuf;
	tbuf.c_lflag &= ~(ECHO | ICANON | ISIG);
	tbuf.c_cc[VMIN] = tbuf.c_cc[VTIME] = 0;
	tcsetattr(0,TCSANOW,&tbuf);
}

/* restore the settings */
void restore_terminal() {
	tcsetattr(0,TCSANOW,&tsave);
}

int main (int argc,char *argv[]) {
	int argindex; char inp = 0;
	int counter = 0;
	
	signal(SIGCHLD,SIG_IGN);
	set_terminal();
	
	Mixer *mixer = new Mixer(argc, argv);
	mixer->printStatus();
	
	while(inp!='+') {
   	mixer->playChunks();
		counter++;
		if (counter == 50) {
			mixer->printStatus();
			counter = 0;
		}
   	inp = getchar();
   	mixer->control(inp);
 	}

	restore_terminal();
	delete mixer; mixer = 0;
	
	return 0;
}

