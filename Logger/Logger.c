#include "stdio.h"
#include "Logger.h"
#include "LinkedList/LinkedList.h"
#include "Threads/Threads.h"

size_t Pow(size_t x, size_t y) {
	return !y ? 1 : y == 1 ? x : x * Pow(x, y - 1);
}

typedef struct {
	TStock* stock;
	FILE* file;
}TLoggerParam;

void LoggerSetTime(TMarket* market, TLog* log)
{
	log->time = clock();
	if (log->time < market->timeControl)
		++market->timeEpoch;
	log->epoch = market->timeEpoch;
}

void LoggerRun(TLoggerParam* param) {
	FILE* file = param->file;
	TStock* stock = param->stock;
	TMarket* begin = stock->market;
	TMarket* end = begin + ITEMS_COUNT;
	while (!stock->stopFlag) {
		for (TMarket* market = begin; market != end; ++market) {
			if (market->log->front) {
				TLogQueue queue;
				size_t divider = (size_t)Pow(10, market->digits);
				Lock(market->loggerMutex);
				LL_Move(market->log, &queue);
				Unlock(market->loggerMutex);
				TLinkedListNode* it = queue.front;
				while (it) {
					TLog* log = (TLog*)it;
					it = it->next;
					switch (log->logType) {
					case LOG_CANCEL:
						fprintf(file,"C,%zu", log->firstOrder);
						break;
					case LOG_DEAL:
						fprintf(file,"T,%zu,%s,%zu,%zu,%zu,%zu.%zu",
							log->dealNumber,
							(log->tradeType == BUY ? "B" : "C"),
							log->firstOrder,
							log->secondOrder,
							log->volume,
							log->price / divider,
							log->price % divider);
						break;
					}
					free(log);
				}
			}

		}
	}
	fclose(file);
	free(param);
}

HANDLE LoggerStart(TStock* stock)
{
	HANDLE log = NULL;
	TLoggerParam* param=(TLoggerParam*)malloc(sizeof(TLoggerParam));
	if (param) {
		param->stock = stock;
		param->file = fopen("output.txt", "w");
		log = ThreadCreate((LPTHREAD_START_ROUTINE)LoggerRun, param, TRUE);
	}
	return log;
}

void LoggerCancel(TMarket* market, TOrder* order)
{
	TLog* log = (TLog*)malloc(sizeof(TLog));
	if (log) {
		LoggerSetTime(market, log);
		log->logType = LOG_CANCEL;
		log->firstOrder = order->id;
		LL_PushBack(market->log, &log->node);
	}
}

void LoggerDeal(TMarket* market, TOrder* first, TOrder* second, size_t volume, size_t dealNumber)
{
	TLog* log = (TLog*)malloc(sizeof(TLog));
	if (log) {
		LoggerSetTime(market, log);
		log->logType = LOG_DEAL;
		log->tradeType = first->type;
		log->digits = market->digits;
		log->price = first->price;
		log->volume = volume;
		log->firstOrder = first->id;
		log->secondOrder = second->id;
		log->dealNumber = dealNumber;
		LL_PushBack(market->log, &log->node);
	}
}
