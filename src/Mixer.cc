#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <signal.h>
#include <sys/types.h>
#include <termios.h>

#include "Mixer.h"

Mixer::Mixer(int argc, char* argv[]) { 
	ch1 = new Channel(argc, argv); 
	ch2 = new Channel(argc, argv); 
	openSoundDevice();
}

Mixer::~Mixer() { 
	ch1->killStream(); 
	ch2->killStream(); 
	closeSoundDevice(); 
}

void Mixer::control(char p) {
	Fader *f;
	switch(p) {
  		case '1': ch1->change(-1); break;
  		case 'q': ch1->change(1); break;
  		case 'w': ch1->change(0); break;
  		case '2': ch1->toggleOnOff(); break;
		case 'a': ch2->change(-1); break;
  		case 'y': ch2->change(1); break;
  		case 'x': ch2->change(0); break;
  		case 's': ch2->toggleOnOff(); break;
		case '3': ch1->incVolume(); printf("%f\n",ch1->getVolume()); break;
		case 'e': ch1->decVolume(); printf("%f\n",ch1->getVolume()); break;
  		case 'd': ch2->incVolume(); printf("%f\n",ch2->getVolume()); break;
  		case 'c': ch2->decVolume(); printf("%f\n",ch2->getVolume()); break;
		case '4': ch1->incSpeed(); printf("%f\n",ch1->getSpeed()); break;
  		case 'r': ch1->decSpeed(); printf("%f\n",ch1->getSpeed()); break;
  		case 'f': ch2->incSpeed(); printf("%f\n",ch2->getSpeed()); break;
  		case 'v': ch2->decSpeed(); printf("%f\n",ch2->getSpeed()); break;
		case '5': if (!ch1->isOn()) ch1->switchOn(); f = new Fader(ch1,8,1.0); f->start(); break;
		case 't': if (!ch1->isOn()) ch1->switchOn(); f = new Fader(ch1,8,0.0); f->start(); break;
		case 'g': if (!ch2->isOn()) ch2->switchOn(); f = new Fader(ch2,8,1.0); f->start(); break;
		case 'b': if (!ch2->isOn()) ch2->switchOn(); f = new Fader(ch2,8,0.0); f->start(); break;
		default: return; 
	}
	printStatus();
}

void Mixer::playChunks() {
	int i, num=BUF_SIZE;

	for (i = 0; i < BUF_SIZE; i++)
		process_buffer[i] = 0;

	ch1->getAudio(process_buffer);
	ch2->getAudio(process_buffer);

	for (i = 0; i < num; i++) {
  		if ((process_buffer[i] < CLIP) && (process_buffer[i] > -CLIP))
  			audio_buffer[i] = (short) (process_buffer[i]);
  		else if (process_buffer[i] > 0) 
  			audio_buffer[i] = MAX_SAMPLE - (CLIP_A / (CLIP_B + process_buffer[i]));
  		else 
  			audio_buffer[i] = -(MAX_SAMPLE - (CLIP_A / (CLIP_B - process_buffer[i])));
	}
	write(audio_fd, audio_buffer, num * 2);
}

void Mixer::openSoundDevice() {
	int format = AFMT_S16_LE, stereo = 1, rate = 44100;

	if ((audio_fd = open("/dev/dsp", O_WRONLY , 0)) == -1) {  
  		perror("/dev/dsp"); exit(1);  
  	}
 	if (ioctl(audio_fd, SNDCTL_DSP_SETFMT, &format)==-1) {  
		perror("SNDCTL_DSP_SETFMT"); exit(1);  
	}
	if (format != AFMT_S16_LE) {
		fprintf(stderr,"Output format not set correctly"); exit(1);
	}
	if (ioctl(audio_fd, SNDCTL_DSP_STEREO, &stereo)==-1) {
		perror("SNDCTL_DSP_STEREO"); exit(1);
	}
	if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &rate)==-1) {
		perror("SNDCTL_DSP_SPEED"); exit(1);  
	}
}

void Mixer::closeSoundDevice() { close(audio_fd); audio_fd = 0; }

void Mixer::printStatus() {
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\nChannel 1: %s\n", ch1->getCurrentSong());
	printf("Status: %s @ %i:%s%i\n",(ch1->isOn() ? "PLAYING" : " ----- "), ch1->getCurrentPosition() / 60, 
			((ch1->getCurrentPosition() % 60) < 10 ? "0" : ""), ch1->getCurrentPosition() % 60);
	printf("Volume: %3.2f \t\t Speed: %3.2f\n\n",ch1->getVolume(),ch1->getSpeed());
	printf("\nChannel 2: %s\n", ch2->getCurrentSong());
	printf("Status: %s @ %i:%s%i\n",(ch2->isOn() ? "PLAYING" : " ----- "), ch2->getCurrentPosition() / 60, 
			((ch2->getCurrentPosition() % 60) < 10 ? "0" : ""), ch2->getCurrentPosition() % 60);
	printf("Volume: %3.2f \t\t Speed: %3.2f\n\n",ch2->getVolume(),ch2->getSpeed());
}

