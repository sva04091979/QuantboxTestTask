#pragma once

#include "Define.h"

HANDLE ThreadCreate(LPTHREAD_START_ROUTINE func, LPVOID param, BOOL isStart);
HANDLE MutexCreate();
void Lock(HANDLE mutex);
void Unlock(HANDLE mutex);