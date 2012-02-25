#include "Thread.h"

#include <iostream>

using namespace std;

Thread::Thread() { }

Thread::~Thread() { }

void Thread::start() {
	pthread_create(&mypthread,NULL,&Thread::start_cb,this);
}

void Thread::quit() { 
	cout << "Thread::quit()\n";
	pthread_cancel(mypthread);
}

void Thread::run() {
	while (1) {
		cout << "Thread::run()\n";
		sleep(1);
	}
}
	
void* (Thread::start_cb) (void *thread) { 
	if (thread) 
		((Thread*) thread)->run();
	return 0;
};

