#pragma once

#include "LinkedList/LinkedList.h"


void MarketStart(TMarket* market);
void MarketPause(TMarket* market, HANDLE mutex);
void MarketClose(TMarket* market);
