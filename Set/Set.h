#pragma once

#include "Define.h"

TOrder* SetRemove(THashSet* set, size_t id);
void SetInsert(THashSet* set, TOrder* order);
void SetFree(THashSet* set);