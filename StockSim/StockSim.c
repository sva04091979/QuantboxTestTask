#include "StockSim.h"
#include "stdlib.h"
#include "Define.h"
#include "Logger/Logger.h"
#include "math.h"

TTradeTask* MakeCancel(char* buf) {
	size_t* id = (size_t*)malloc(sizeof(size_t));
	if (id) {
		char* end;
		*id = strtoul(buf, &end, 10);
		TTradeTask* task = (TTradeTask*)malloc(sizeof(TTradeTask));
		if (task) {
			task->taskType = ORDER_CANCEL;
			task->market = APPLE;
			task->task = id;
			return task;
		}
		free(id);
	}
	return NULL;
}

TTradeTask* MakeOrder(TStock* stock,char* buf) {
	TOrder* order = (TOrder*)malloc(sizeof(TOrder));
	if (order) {
		order->market = APPLE;
		char* end;
		order->id_forControl = strtoul(buf, &end, 10);
		if (*end == ',') {
			buf = end + 1;
			BOOL isOk = TRUE;
			switch (*buf) {
			case 'B':
				order->type = BUY;
				break;
			case 'S':
				order->type = SELL;
				break;
			default:
				isOk = FALSE;
				break;
			}
			if (isOk && *++buf == ',') {
				order->volume = strtoul(++buf, &end, 10);
				if (*end == ',') {
					buf = end + 1;
					size_t p = Pow(10, digits[order->market]);
					double price = strtod(buf, &end);
					order->price = lround(price * p);
					if (*end == '\n') {
						TTradeTask* task = (TTradeTask*)malloc(sizeof(TTradeTask));
						if (task) {
							order->id = InterlockedInc(&stock->ordersNumber);
							if (order->id != order->id_forControl) {
								printf("Id error. My=%zu, origin=%zu", order->id, order->id_forControl);
							}
							task->taskType = ORDER_SEND;
							task->market = APPLE;
							task->task = order;
							return task;
						}
					}
				}
			}
		}
	}
	free(order);
	return NULL;
}

TTradeTask* Parse(TStock* stock,FILE* file) {
	char buf[256];
	if (NULL != fgets(buf, sizeof buf, file)) {
		switch (*buf) {
		case 'O':
			return MakeOrder(stock,buf + 2);
		case 'C':
			return MakeCancel(buf + 2);
		default:
			return NULL;
		}
	}
	return NULL;
}

int main()
{
	void(*task)(TStock*,TTradeQueue*)=NULL;
	TStock* stock=StockStart(&task);
	TTradeQueue* queue = (TTradeQueue*)calloc(1, sizeof(TTradeQueue));
	FILE* file=NULL;
	if (!fopen_s(&file, "input.txt", "r")) {
		while (!feof(file)) {
			TTradeTask* task = Parse(stock,file);
			if (task) {
				LL_PushBack(queue, &task->node);
			}
			else {
				if (!feof(file))
					puts("parse error");
			}
		}
	}
	task(stock, queue);
	StockStop(stock);
	getch();
	return 0;
}
