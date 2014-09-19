#include "StdAfx.h"
#include "CLock.h"
#include <iostream>

#ifndef WIN32
#define exit(x) _exit(x)
#endif
using namespace std;

CLock :: CLock(){
	#ifdef WIN32
		//InitializeCriticalSection(&g_CriticalSection);
		hdLock = CreateMutex(NULL,FALSE,NULL);
		if(hdLock == NULL){
			cout << "LOCK CONSTRUCTION ERROR!" << endl;
			exit(0);
		}
		
	#else
		if(pthread_mutex_init (&mutexLock, NULL) != 0){
			cout << "LOCK CONSTRUCTION ERROR!" << endl;
			exit(0);
		}
	#endif
}

void CLock :: Lock(){
	#ifdef WIN32
		//EnterCriticalSection(&g_CriticalSection);
		//may have problem here
		dwWaitResult = WaitForSingleObject(hdLock, INFINITE);
		if(dwWaitResult == WAIT_OBJECT_0){
			//cout <<hdLock<< "lockd" << endl;
			return;
		}
		else{
			cout << "LOCK ERROR!" << endl;
			exit(0);
		}
		
	#else
		if(pthread_mutex_lock(&mutexLock) != 0){
			cout << "LOCK ERROR!" << endl;
			exit(0);
		}
	#endif
}

void CLock :: Unlock(){
	#ifdef WIN32
		//LeaveCriticalSection(&g_CriticalSection);
		if(ReleaseMutex(hdLock)){
			//cout <<hdLock<< "unlock" << endl;
			return;
		}
		else{
			cout << "UNLOCK ERROR!" << endl;
			exit(0);
		}
	#else
		if(pthread_mutex_unlock(&mutexLock) != 0){
			cout << "UNLOCK ERROR!" << endl;
			exit(0);
		}
	#endif
}

CLock :: ~CLock(){
	#ifdef WIN32
		//DeleteCriticalSection(&g_CriticalSection);  
		CloseHandle(hdLock);
		//
	#else
		 pthread_mutex_destroy(&mutexLock);
	#endif
}
