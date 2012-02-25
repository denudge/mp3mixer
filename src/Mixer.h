#ifndef __MIXER_H__
#define __MIXER_H__

#include "Channel.h"
#include "Fader.h"

#define CLIP        30000
#define CLIP_A      ((MAX_SAMPLE-CLIP)*(MAX_SAMPLE-CLIP))
#define CLIP_B      (MAX_SAMPLE-2*CLIP)

class Mixer {
	public:
		Mixer(int argc, char* argv[]);
		~Mixer();

		void control(char p);
		void playChunks();
	
		void printStatus();

	private:
		Channel *ch1;
		Channel *ch2;
		
		int audio_fd;  
		short audio_buffer[BUF_SIZE];  
		int process_buffer[BUF_SIZE];

		void openSoundDevice();
		void closeSoundDevice();
}; // end of class Mixer

#endif

