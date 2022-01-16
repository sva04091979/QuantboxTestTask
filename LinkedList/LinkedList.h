#pragma once

#include "Define.h"

TLinkedListNode* LL_PushBack(TLinkedList* list,TLinkedListNode* node);
TLinkedListNode* LL_PushFront(TLinkedList* list, TLinkedListNode* node);
TLinkedListNode* LL_PopFront(TLinkedList* list);
TLinkedListNode* LL_Remove(TLinkedListNode* node);
TLinkedListNode* LL_Front(TLinkedList* list);
TLinkedListNode* LL_Back(TLinkedList* list);
void LL_InsertAfter(TLinkedListNode* target, TLinkedListNode* node);
void LL_InsertBefore(TLinkedListNode* target, TLinkedListNode* node);
void LL_Move(TLinkedList* from, TLinkedList* to);