#include "StockSim.h"
#include "stdlib.h"
#include "Define.h"
#include "Logger/Logger.h"

TTradeTask* MakeCancel(char* buf) {
	size_t* id = (size_t*)malloc(sizeof(size_t));
	if (id) {
		char* end;
		*id = strtuul(buf, &end, 10);
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

TTradeTask* MakeOrder(char* buf) {
	TOrder* order = (TOrder*)malloc(sizeof(TOrder));
	if (order) {
		order->market = APPLE;
		char* end;
		order->id_forControl = strtoul(buf, &end, 10);
		if (*end == ',') {
			BOOL isOk = TRUE;
			switch (*++end) {
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
			if (isOk && ++end == ',') {
				order->volume=strtoul(++end,&end)
			}
		}
		TTradeTask* task = (TTradeTask*)malloc(sizeof(TTradeTask));
		if (task) {
			task->taskType = ORDER_CANCEL;
			task->market = APPLE;
			task->task = id;
			return task;
		}
		else
			free(id);
	}
	return NULL;
}

TTradeTask* Parse(FILE* file) {
	char buf[256];
	if (NULL != fgets(buf, sizeof buf, file)) {
		switch (*buf) {
		case 'O':
			return MakeOrder(buf + 2);
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
	void(*task)(TStock*,TTradeTask*,size_t)=NULL;
	TStock* stock=StockStart(&task);
	TTradeQueue* queue = (TTradeQueue*)calloc(1, sizeof(TTradeQueue));
	FILE* file=NULL;
	if (!fopen_s(&file, "input.txt", "r")) {
		while (!feof(file)) {
			TTradeTask* task = Parse(file);
			if (task)
				LL_PushBack(queue, &task->node);
			else
				puts("parse error");
		}
	}
	StockStop(stock);
	return 0;
}
