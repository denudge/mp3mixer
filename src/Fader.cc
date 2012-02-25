#include "Fader.h"
#include <unistd.h>

Fader::Fader(Channel *channel, unsigned int seconds, float end_value) {
	if (!channel) delete this;
	this->channel = channel;
	this->seconds = seconds;
	this->start_value = channel->getVolume();
	this->end_value = end_value;
	if (start_value == end_value)
		step =0.0;
	else
		this->step = (end_value - start_value) / (float) (seconds*10.0f);
}

void Fader::run() {
	if (!channel->setFader()) ;
	for (int c=0; c < (seconds*10); c++) {
		channel->incVolume(step);
		usleep(100000);
	}
	channel->setVolume(end_value);
	channel->freeFader();
	quit();
	delete this;
}

