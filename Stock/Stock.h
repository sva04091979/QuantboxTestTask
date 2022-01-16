#pragma once

#include "Market.h"
extern const size_t* digits;

TStock* StockStart(void(**task)(TStock*, TTradeTask*, size_t));
void StockStop(TStock* stock);