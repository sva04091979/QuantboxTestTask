#include "Set.h"
#include "LinkedList/LinkedList.h"

TOrder* SetRemove(THashSet* set, size_t id) {
	THashNode* node = &set->hash[id % HASH_SIZE];
	THashObject* it = (THashObject*)node->front;
	while (it) {
		if (it->order->id == id) {
			TOrder* order = it->order;
			LL_Remove(&it->node);
			free(it);
			return order;
		}
		it = (THashObject*)it->node.next;
	}
	return NULL;
}

void SetInsert(THashSet* set, TOrder* order)
{
	THashObject* object = (THashObject*)malloc(sizeof(THashObject));
	if (object) {
		THashNode* node = &set->hash[order->id % HASH_SIZE];
		object->order = order;
		LL_PushBack(node, &object->node);
	}
}

