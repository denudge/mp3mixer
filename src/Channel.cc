#include "Channel.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <sys/types.h>
#include <signal.h>

Channel::Channel(int _argc, char* _argv[]) {
	init(); 
	argc = _argc; 
	argv = _argv;
	change(0);
}

Channel::~Channel() { 
	if (fstr) { 
		fclose(fstr); 
		fstr=0;	
	}
}

void Channel::init() { 
	clearBuffer(); 
	on=0; 
	stream=0;
	volume=1.0f;
	speed=1.0f;
	fstr=0;
	argindx=1;
	position = 0;
	need_spawn=1; 
}

void Channel::killStream() {
	if (pid > 0) {
		kill(pid,15);
	} else { 
		if (stream > 2) 
			close(stream);
	}
	pid=0;
	on=0;
	position = 0;
	clearBuffer();
}

// step up through the list of tracks
void Channel::change(int inc) {
	killStream();
	argindx += inc;
	if (argindx >= argc)
		argindx=1;
	else if (argindx < 1)
  		argindx=argc-1;

	speed=1.0f;
	need_spawn = 1;
}

char* Channel::getCurrentSong() {
	return argv[argindx];
}

int Channel::getCurrentPosition() {
	return (int) (position / 44100);
}

void Channel::spawnStream(char *filename) {
	int strm[2];
	pid_t new_pid;

	// create pipe
	pipe(strm);
 	new_pid=fork();
 	if (new_pid) {
  		pid = new_pid;
  		close(strm[1]);
  		stream = strm[0];
  		fstr = fdopen(stream,"rb");
 	} else {
  		close(strm[0]);
  		close(1);
  		dup(strm[1]);
  		close(strm[0]);
  		close(strm[1]);
  		execlp("mpg123","mpg123","-q","-s",filename,NULL);
 	}
}

void Channel::getAudio(int *buf) {
	static short adat[BUF_SIZE], inbuf[BUF_SIZE*4]; int i,j;
	if (!on) return;
  				
	if (speed == 1.0)
		i = fread(adat, 2, BUF_SIZE, fstr);
 	else {
  		j= (int) ((BUF_SIZE>>1) * speed);
  		j<<=1;
  		i = fread(inbuf, 2, j, fstr);
  		if (i < j) {
  			killStream();
  			for( ; i < j ; i++)
				inbuf[i]=0;
		}
  		for (i = 0; i < (BUF_SIZE/2); i++) {
  			j = (int) (i * speed);
  			adat[i*2] = inbuf[j*2];
  			adat[i*2+1] = inbuf[j*2+1];
  		}
  		i = BUF_SIZE;
 	}
 	if (i < BUF_SIZE) {
  		killStream();
  		for ( ; i < BUF_SIZE; i++)
  			adat[i]=0;
	}
	for(i=0; i < BUF_SIZE; i++)
     buf[i] += (int) (adat[i] * volume);
	position += BUF_SIZE / 2;
}

float Channel::getVolume() { return volume; }
void  Channel::incVolume(float step) { volume += step; if (volume > 2.0) volume = 2.0; }
void  Channel::decVolume(float step) { volume -= step; if (volume < 0.0) volume = 0.0; }
float Channel::getSpeed() { return speed; }
void  Channel::incSpeed() { speed += 0.01; }
void  Channel::decSpeed() { speed -= 0.01; }
int  Channel::isOn() { return on; }
void Channel::switchOn() { if (!on) on=1; if (need_spawn) { spawnStream(argv[argindx]); need_spawn = 0; } }
void Channel::switchOff() { if (on) on=0; }
void Channel::toggleOnOff() { if (on) on=0; else switchOn(); }
void Channel::clearBuffer() { for (int i=0; i < BUF_SIZE; i++) inbuf[i]=0; }		

