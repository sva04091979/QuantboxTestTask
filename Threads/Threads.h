#pragma once

#include "Define.h"

THREAD ThreadCreate(FUNC_PTR func, LPVOID param, BOOL isStart);
MUTEX MutexCreate();
void Lock(MUTEX mutex);
void Unlock(THREAD mutex);