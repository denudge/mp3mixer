#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <stdio.h>
#include <sys/types.h>

#define BUF_SIZE    4096 /* this is set to this value because it's the longest fragment we can read from a pipe  */
#define MAX_SAMPLE  32766

class Channel {
	public:
		Channel(int _argc, char* _argv[]);
		~Channel();

		void killStream();
		void change(int inc);
		char* getCurrentSong();
		int getCurrentPosition();

		// get the audio for one channel - should include speedup and slowdown here
		void getAudio(int *buf);

		float getVolume();
		void  incVolume(float step = 0.05);
		void  decVolume(float step = 0.05);
		void  setVolume(float value) { volume = value; if (volume < 0) volume=0.0; else if (volume > 2.0) volume=2.0; }

		float getSpeed();
		void  incSpeed();
		void  decSpeed();

		int  isOn();
		void switchOn();
		void switchOff();
		void toggleOnOff();
		
		int setFader() { if (fader_active) return 0; else fader_active = 1; return 1; }
		void freeFader() { fader_active = 0; }

	private:
		int fader_active;
		short inbuf[BUF_SIZE];
		float volume, speed;
		int on, need_spawn, stream;
		int position;
		FILE *fstr;
		pid_t pid;
		int argindx;
		int argc;
		char** argv;
		
		void init();
		void clearBuffer();
		void spawnStream(char *filename);

}; // end of class Channel

#endif

