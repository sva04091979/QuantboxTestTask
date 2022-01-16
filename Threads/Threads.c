// Threads.cpp: исходный файл для целевого объекта.
//

#include "Threads.h"

HANDLE ThreadCreate(LPTHREAD_START_ROUTINE func, LPVOID param,BOOL isStart)
{
	DWORD flag = isStart ? 0 : CREATE_SUSPENDED;
	return CreateThread(NULL, 0, func, param, flag, NULL);
}

HANDLE MutexCreate()
{
	return CreateMutex(NULL,FALSE,NULL);
}

void Lock(HANDLE mutex)
{
	WaitForSingleObject(mutex, INFINITE);
}

void Unlock(HANDLE mutex)
{
	ReleaseMutex(mutex);
}
