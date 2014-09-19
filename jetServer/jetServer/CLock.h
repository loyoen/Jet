#ifndef CLOCK_H
#define CLOCK_H

#ifdef WIN32
	#include <windows.h>
	#include <stdio.h>
#else
	#include<pthread.h>
#endif

class CLock{
	public:
		CLock();
		void Lock();
		void Unlock();
		~CLock();

	private:
		#ifdef WIN32
			//CRITICAL_SECTION g_CriticalSection; 
			HANDLE hdLock;
			DWORD dwWaitResult;
		#else
			pthread_mutex_t mutexLock;
		#endif
};
#endif