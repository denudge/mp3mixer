#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class Thread {
	public:
		Thread();
		virtual ~Thread();

		void start();
		void quit();

	protected:
		virtual void run();
	
	private:
		pthread_t mypthread;
		static void* (start_cb) (void *thread);
};

#endif

