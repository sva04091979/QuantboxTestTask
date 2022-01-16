#pragma once

#include "stddef.h"
#include "time.h"
#include <windows.h>

#define HASH_SIZE 1024

#ifdef _WIN64
	#define InterlockedInc InterlockedIncrement64
#else
	#define InterlockedInc InterlockedIncrement
#endif

typedef enum {
	BUY,
	SELL
} TRADE_TYPE;

typedef enum {
	APPLE,
	ITEMS_COUNT
} TRADE_ITEM;

typedef enum {
	ORDER_SEND,
	ORDER_CANCEL
} TRADE_TASK;

typedef enum {
	LOG_DEAL,
	LOG_CANCEL
} LOG_TYPE;

typedef struct _TLinkedListNode TLinkedListNode;
typedef struct _TLinkedList TLinkedList;
typedef struct _TPriceLevel TPriceLevel;
typedef struct _TStock TStock;

typedef struct _TLinkedListNode {
	TLinkedListNode* prev;
	TLinkedListNode* next;
	TLinkedList* list;
} TLinkedListNode;

typedef struct _TLinkedList {
	TLinkedListNode* front;
	TLinkedListNode* back;
} TLinkedList, THashNode, TTradeQueue, TLogQueue;

typedef struct {
	TLinkedListNode node;
	TPriceLevel* priceLevel;
	size_t price;
	size_t volume;
	size_t id;
	size_t id_forControl;
	TRADE_ITEM market;
	TRADE_TYPE type;
} TOrder;

typedef struct {
	TLinkedListNode node;
	TOrder* order;
} THashObject;

typedef struct {
	THashNode hash[HASH_SIZE];
} THashSet;

typedef struct _TPriceLevel {
	TLinkedListNode node;
	TLinkedList queqe;
} TPriceLevel;

typedef struct {
	TLinkedList list;
	BOOL(*_max)(size_t,size_t);
	BOOL(*_min)(size_t, size_t);
} TTradeSide;

typedef struct {
	TLinkedListNode node;
	void* task;
	TRADE_ITEM market;
	TRADE_TASK taskType;
} TTradeTask;

typedef struct {
	TLinkedListNode node;
	size_t firstOrder;
	size_t secondOrder;
	size_t dealNumber;
	size_t price;
	size_t volume;
	size_t digits;
	size_t epoch;
	clock_t time;
	TRADE_TYPE tradeType;
	LOG_TYPE logType;
}TLog;

typedef struct {
	TTradeSide* sell;
	TTradeSide* buy;
	TLogQueue* log;
	TTradeQueue* queue;
	THashSet* set;
	TStock* stock;
	HANDLE taskMutex;
	HANDLE loggerMutex;
	HANDLE thread;
	size_t digits;
	size_t timeEpoch;
	clock_t timeControl;
	BOOL stopFlag;
} TMarket;

typedef struct _TStock{
	size_t dealsNumber;
	size_t ordersNumber;
	TMarket market[ITEMS_COUNT];
	HANDLE logger;
	BOOL stopFlag;
} TStock;

