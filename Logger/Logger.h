#pragma once

#include "Define.h"

size_t Pow(size_t x, size_t y);
HANDLE LoggerStart(TStock* stock);
void LoggerCancel(TMarket* market, TOrder* order);
void LoggerDeal(TMarket* market, TOrder* first, TOrder* second, size_t volume, size_t dealNumber);