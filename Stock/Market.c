#include "Market.h"
#include "Set/Set.h"
#include "Threads/Threads.h"
#include "Logger/Logger.h"

BOOL Max(size_t l, size_t r) {
	return l==max(l, r);
}

BOOL Min(size_t l, size_t r) {
	return l==min(l, r);
}

size_t Deal(TMarket* market, TOrder* order) {
	BOOL(*_max)(size_t, size_t);
	BOOL(*_min)(size_t, size_t);
	TTradeSide* side = order->type == BUY ? market->sell : market->buy;
	TPriceLevel* front = (TPriceLevel*)side->list.front;
	_max = side->_max;
	_min = side->_min;
	while (front&&order->volume>0){
		TOrder* first = (TOrder*)front->queqe.front;
		if (first) {
			if (_min(order->price, first->price))
				break;
			while (first && order->volume > 0) {
				size_t delta = min(first->volume, order->volume);
				LoggerDeal(market,first, order, delta, InterlockedInc(&market->stock->dealsNumber));
				first->volume -= delta;
				order->volume -= delta;
				if (!first->volume) {
					SetRemove(market->set, first->id);
					first = (TOrder*)first->node.next;
					free(LL_PopFront(&front->queqe));
				}
			}
		}
		front = (TPriceLevel*)front->node.next;
		if (!first) {
			free(LL_PopFront(&side->list));
		}
	}
	size_t ret = order->volume;
	if (!ret)
		free(order);
	return ret;
}

void CancelOrder(TMarket* market,const size_t* id) {
	TOrder* order = SetRemove(market->set,*id);
	TPriceLevel* level = order->priceLevel;
	LL_Remove(&order->node);
	if (!level->queqe.front)
		LL_Remove(&level->node);
	LoggerCancel(market,order);
	free(order);
}

void Pending(TMarket* market, TOrder* order) {
	BOOL(*_max)(size_t, size_t);
	BOOL(*_min)(size_t, size_t);
	TTradeSide* side = order->type == BUY ? market->buy : market->sell;
	TPriceLevel* back = (TPriceLevel*)side->list.back;
	TPriceLevel* front = (TPriceLevel*)side->list.front;
	_max = side->_max;
	_min = side->_min;
	SetInsert(market->set, order);
	while (TRUE) {
		if (order->price == ((TOrder*)front->queqe.front)->price) {
			LL_PushBack(&front->queqe, &order->node);
			break;
		}
		else if (order->price == ((TOrder*)back->queqe.front)->price) {
			LL_PushBack(&back->queqe, &order->node);
			break;
		}
		else if (_min(order->price, ((TOrder*)front->queqe.front)->price)) {
			TPriceLevel* level = (TPriceLevel*)malloc(sizeof(TPriceLevel));
			if (level) {
				LL_PushBack(&level->queqe, &order->node);
				LL_InsertBefore(&front->node, &level->node);
			}
			break;
		}
		else if (_max(order->price, ((TOrder*)back->queqe.front)->price)) {
			TPriceLevel* level = (TPriceLevel*)malloc(sizeof(TPriceLevel));
			if (level) {
				LL_PushBack(&level->queqe, &order->node);
				LL_InsertAfter(&back->node, &level->node);
			}
			break;
		}
		++front;
		--back;
	}
}

void MarketRun(TMarket* market) {
	while (!market->stopFlag) {
		if (market->queue->front) {
			Lock(market->loggerMutex);
			Lock(market->taskMutex);
			TLinkedList queue;
			LL_Move(market->queue, &queue);
			Unlock(market->taskMutex);
			TTradeTask* task = (TTradeTask*)queue.front;
			while (task) {
				switch (task->taskType) {
				case ORDER_SEND:
					if (Deal(market,task->task) > 0)
						Pending(market,task->task);
					break;
				case ORDER_CANCEL:
					CancelOrder(market,task->task);
					break;
				}
				TTradeTask* tmp = task;
				task = (TTradeTask*)task->node.next;
				free(tmp);
			}
			Unlock(market->loggerMutex);
		}
	}
}

void MarketStart(TMarket* market)
{
	if (NULL!=(market->buy = (TTradeSide*)calloc(1, sizeof(TTradeSide)))&&
		NULL!=(market->sell = (TTradeSide*)calloc(1, sizeof(TTradeSide)))) {
			market->buy->_max = Min;
			market->buy->_min = Max;
			market->sell->_max = Max;
			market->sell->_min = Min;
	}
	market->stopFlag = FALSE;
	market->timeEpoch = 0;
	market->timeControl = clock();
	market->loggerMutex = MutexCreate();
	market->taskMutex = MutexCreate();
	market->set = (THashSet*)calloc(1, sizeof(THashSet));
	market->queue= (TTradeQueue*)calloc(1, sizeof(TTradeQueue));
	market->log = (TLogQueue*)calloc(1, sizeof(TLogQueue));
	market->thread=ThreadCreate((LPTHREAD_START_ROUTINE)MarketRun, market,FALSE);
	ResumeThread(market->thread);
}

void PriceLevelFree(TPriceLevel* level) {
	TLinkedListNode* it = level->queqe.front;
	while (it) {
		TOrder* order = (TOrder*)it;
		it = it->next;
		free(order);
	}
	free(level);
}

void SideFree(TTradeSide* side) {
	TLinkedListNode* it = side->list.front;
	while (it) {
		TPriceLevel* level = (TPriceLevel*)it;
		it = it->next;
		PriceLevelFree(level);
	}
	free(side);
}

void TasksFree(TTradeQueue* queue) {
	TLinkedListNode* it = queue->front;
	while (it) {
		TTradeTask* task = (TTradeTask*)it;
		it = it->next;
		free(task->task);
		free(task);
	}
	free(queue);
}

void LogsFree(TLogQueue* queue) {
	TLinkedListNode* it = queue->front;
	while (it) {
		TLog* log = (TLog*)it;
		it = it->next;
		free(log);
	}
	free(queue);
}

void MarketStop(TMarket* market)
{
	market->stopFlag = TRUE;
	WaitForSingleObject(market->thread, INFINITE);
	CloseHandle(market->thread);
	CloseHandle(market->loggerMutex);
	CloseHandle(market->taskMutex);
	free(market->set);
	SideFree(market->buy);
	SideFree(market->sell);
	TasksFree(market->queue);
	LogsFree(market->log);
}
