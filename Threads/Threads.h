#pragma once

#include "Define.h"

THREAD ThreadCreate(LPTHREAD_START_ROUTINE func, LPVOID param, BOOL isStart);
MUTEX MutexCreate();
void Lock(MUTEX mutex);
void Unlock(THREAD mutex);