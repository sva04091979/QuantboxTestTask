// Stock.cpp: исходный файл для целевого объекта.
//

#include "Stock.h"
#include <stdio.h>
#include <stdlib.h>
#include "Threads/Threads.h"
#include "Logger/Logger.h"

const size_t digits[ITEMS_COUNT] = { 2 };

void SendTask(TStock* stock, TTradeTask* task,size_t count) {
	if (count > 0) {
		TTradeTask* end = task + count;
		HANDLE curMutex = stock->market[task->market].taskMutex;
		Lock(curMutex);
		while (task != end) {
			TMarket* market = &stock->market[task->market];
			HANDLE mutex = market->taskMutex;
			if (mutex != curMutex) {
				Unlock(curMutex);
				curMutex = mutex;
				Lock(curMutex);
			}
			LL_PushBack(market->queue, &task->node);
			++task;
		}
		Unlock(curMutex);
	}
}

TStock* StockStart(void(**task)(TStock*,TTradeTask*,size_t))
{
	TStock* stock = (TStock*)_aligned_malloc(sizeof(TStock),64);
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
	stock->stopFlag = TRUE;
	WaitForSingleObject(stock->logger, INFINITE);
	CloseHandle(stock->logger);
	for (size_t i = 0; i < ITEMS_COUNT; ++i) {
		MarketStop(&stock->market[i]);
	}
	_aligned_free(stock);
}
