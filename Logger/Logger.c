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

void Check(FILE* file) {
	FILE* _check = fopen("check.txt", "r");
	FILE* check = fopen("MyCheck.txt", "w+b");
	if (_check && check) {
		int i = -1;
		while (!feof(_check)) {
			char c = fgetc(_check);
			if (i > -1)
				++i;
			if (c == '.')
				i = 0;
			if (c == '\n') {
				if (i > -1 && i < 3) {
					while (i++ < 3) {
						fputc('0', check);
					}
				}
				i = -1;
			}
			fputc(c, check);
		}
		rewind(file);
		rewind(check);
		i = 0;
		while (!feof(file) && !feof(check)) {
			++i;
			char f = fgetc(file);
			char c = fgetc(check);
			if (f != c) {
				printf("%i!=%i\n", f, c);
				printf("Error in pos %i,%i\n", ftell(file), ftell(check));
				return;
			}
		}
		puts("Ok");
	}
	else {
		puts("Check file open error");
	}
}

void LoggerRun(TLoggerParam* param) {
	static char mess[] = "T,%zu,%s,%zu,%zu,%zu,%zu.%.00zu\n";
	FILE* file = param->file;
	TStock* stock = param->stock;
	TMarket* begin = stock->market;
	TMarket* end = begin + ITEMS_COUNT;
	while (!stock->stopFlag) {
		for (TMarket* market = begin; market != end; ++market) {
			if (market->log->front) {
				mess[27] = market->digits / 10 + 48;
				mess[28] = market->digits + 48;
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
						fprintf(file,"X,%zu\n", log->firstOrder);
						break;
					case LOG_DEAL:
						fprintf(file, mess,
							log->dealNumber,
							(log->tradeType == BUY ? "B" : "S"),
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
	fflush(file);
#ifdef _DEBUG
	Check(file);
#endif
	fclose(file);
	free(param);
}

THREAD LoggerStart(TStock* stock)
{
	THREAD log = NULL;
	TLoggerParam* param=(TLoggerParam*)malloc(sizeof(TLoggerParam));
	if (param) {
		param->stock = stock;
		param->file = fopen("output.txt", "w+b");
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
