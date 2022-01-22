// Stock.cpp: исходный файл для целевого объекта.
//

#include "Stock.h"
#include <stdio.h>
#include <stdlib.h>
#include "Threads/Threads.h"
#include "Logger/Logger.h"

const size_t digits[ITEMS_COUNT] = { 2 };

void SendTask(TStock* stock, TTradeQueue* queue) {
	TTradeTask* it = (TTradeTask*)queue->front;
	if (it) {
		MUTEX curMutex = stock->market[it->market].taskMutex;
		Lock(curMutex);
		while (it) {
			TMarket* market = &stock->market[it->market];
			MUTEX mutex = market->taskMutex;
			if (mutex != curMutex) {
				Unlock(curMutex);
				curMutex = mutex;
				Lock(curMutex);
			}
			TTradeTask* task = it;
			it = (TTradeTask*)it->node.next;
			LL_PushBack(market->queue, &task->node);
		}
		Unlock(curMutex);
	}
	free(queue);
}

TStock* StockStart(void(**task)(TStock*,TTradeQueue*))
{
	TStock* stock = (TStock*)_aligned_malloc(sizeof(TStock),sizeof(size_t));
	if (stock) {
		*task = SendTask;
		stock->ordersNumber = 0;
		stock->dealsNumber= 0;
		stock->stopFlag = FALSE;
		TMarket* market = stock->market;
		TMarket* end=market+ITEMS_COUNT;
		const size_t* dg = digits;
		while(market<end){
			market->digits = *dg++;
			market->stock = stock;
			MarketStart(market++);
		}
		stock->logger = LoggerStart(stock);
	}
	return stock;
}

void StockStop(TStock* stock)
{
	THREAD pause[ITEMS_COUNT];
	for (size_t i = 0; i < ITEMS_COUNT; ++i) {
		pause[i] = stock->market[i].thread;
		MarketPause(&stock->market[i]);
	}
	WaitForMultipleObjects(ITEMS_COUNT, pause, TRUE, INFINITE);
	stock->stopFlag = TRUE;
	WaitForSingleObject(stock->logger, INFINITE);
	CloseHandle(stock->logger);
	for (size_t i = 0; i < ITEMS_COUNT; ++i) {
		MarketClose(&stock->market[i]);
	}
	_aligned_free(stock);
}
