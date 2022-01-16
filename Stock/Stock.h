#pragma once

#include "Market.h"

const size_t digits[ITEMS_COUNT];

TStock* StockStart(void(**task)(TStock*, TTradeQueue*));
void StockStop(TStock* stock);