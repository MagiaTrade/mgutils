//
// Created by Arthur Motelevicz on 21/10/24.
//

#ifndef MMDPERSISTOR_TRADE_H
#define MMDPERSISTOR_TRADE_H

#include <string>
#include "Utils.h"

namespace mgutils::models
{
  struct Trade
  {
    std::string source;
    std::string symbol;
    double price = dNaN;
    double amount = dNaN;
    char makerSide = INVALID_CHAR; // Valid values: B, S or U (buy, sell or undefined)
    int64_t time = INVALID_INT64;
  };

  enum Side
  {
    UNDEFINED = 'U',
    BUY = 'B',
    SELL = 'S',
  };
}

#endif //MMDPERSISTOR_TRADE_H
