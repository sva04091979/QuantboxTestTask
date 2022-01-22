// Threads.cpp: исходный файл для целевого объекта.
//

#include "Threads.h"

THREAD ThreadCreate(LPTHREAD_START_ROUTINE func, LPVOID param,BOOL isStart)
{
	DWORD flag = isStart ? 0 : CREATE_SUSPENDED;
	return CreateThread(NULL, 0, func, param, flag, NULL);
}

MUTEX MutexCreate()
{
	return CreateMutex(NULL,FALSE,NULL);
}

void Lock(MUTEX mutex)
{
	WaitForSingleObject(mutex, INFINITE);
}

void Unlock(MUTEX mutex)
{
	ReleaseMutex(mutex);
}
