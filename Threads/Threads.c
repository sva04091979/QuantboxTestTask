// Threads.cpp: исходный файл для целевого объекта.
//

#include "Threads.h"

THREAD ThreadCreate(FUNC_PTR func, LPVOID param,BOOL isStart)
{
#ifdef _WIN32
	DWORD flag = isStart ? 0 : CREATE_SUSPENDED;
	return CreateThread(NULL, 0, func, param, flag, NULL);
#endif
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
