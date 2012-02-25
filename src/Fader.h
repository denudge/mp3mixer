#ifndef __FADER_H__
#define __FADER_H__

#include "Thread.h"
#include "Channel.h"

class Fader : public Thread {
	public:
		Fader(Channel *channel, unsigned int seconds, float end_value);
		virtual ~Fader() { }

	protected:
		void run();

	private:
		float start_value, end_value, step;
		int seconds;
		Channel *channel;
};

#endif

