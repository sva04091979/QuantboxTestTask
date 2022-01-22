#include "LinkedList.h"

TLinkedListNode* LL_PushBack(TLinkedList* list, TLinkedListNode* node)
{
	node->list = list;
	node->next = NULL;
	node->prev = list->back;
	if (!list->front)
		list->front = node;
	else
		list->back->next = node;
	list->back = node;
	return node;
}

TLinkedListNode* LL_PushFront(TLinkedList* list, TLinkedListNode* node)
{
	node->list = list;
	node->next = list->front;
	node->prev = NULL;
	if (!list->back)
		list->back = node;
	else
		list->front->prev = node;
	list->front = node;
	return node;
}

TLinkedListNode* LL_PopFront(TLinkedList* list)
{
	if (!list->front)
		return NULL;
	TLinkedListNode* node = list->front;
	list->front = list->front->next;
	if (!list->front)
		list->back = NULL;
	else
		list->front->prev = NULL;
	return node;
}

TLinkedListNode* LL_Remove(TLinkedListNode* node)
{
	TLinkedList* list = node->list;
	TLinkedListNode* prev = node->prev;
	TLinkedListNode* next = node->next;
	node->list = NULL;
	node->prev = NULL;
	node->next = NULL;
	if (prev)
		prev->next = next;
	else
		list->front = next;
	if (next)
		next->prev = prev;
	else
		list->back = prev;
	return next;
}

TLinkedListNode* LL_Front(TLinkedList* list)
{
	return list->front;
}

TLinkedListNode* LL_Back(TLinkedList* list)
{
	return list->back;
}

void LL_InsertAfter(TLinkedListNode* target, TLinkedListNode* node)
{
	TLinkedListNode* next = target->next;
	if (!next)
		LL_PushBack(target->list, node);
	else {
		node->list = target->list;
		target->next = node;
		next->prev = node;
		node->prev = target;
		node->next = next;
	}
}

void LL_InsertBefore(TLinkedListNode* target, TLinkedListNode* node)
{
	TLinkedListNode* prev = target->prev;
	if (!prev)
		LL_PushFront(target->list, node);
	else {
		node->list = target->list;
		target->prev = node;
		prev->next = node;
		node->prev = prev;
		node->next = target;
	}
}

void LL_Move(TLinkedList* from, TLinkedList* to)
{
	to->front = from->front;
	to->back = from->back;
	from->front = NULL;
	from->back = NULL;
}
