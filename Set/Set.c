#include "Set.h"
#include "LinkedList/LinkedList.h"

TOrder* SetRemove(THashSet* set, size_t id) {
	THashNode* node = &set->hash[id % HASH_SIZE];
	THashObject* it = (THashObject*)node->front;
	while (it) {
		if (it->order->id == id) {
			LL_Remove(&it->node);
			TOrder* order = it->order;
			free(it);
			return order;
		}
		it = (THashObject*)it->node.next;
	}
	return NULL;
}

void SetInsert(THashSet* set, TOrder* order)
{
	THashNode* node = &set->hash[order->id % HASH_SIZE];
	LL_PushBack(node, &order->node);
}

