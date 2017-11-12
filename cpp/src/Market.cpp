//
// Created by lukasz on 11.11.17.
//

#include "Market.h"

#include <locale>
#include <iostream>

Market::Market(nlohmann::json j)
{
    setCurrency(j["MarketCurrency"]);
    setCurrencyLong(j["MarketCurrencyLong"]);
    setMarketName(j["MarketName"]);
    setIsActive(j["IsActive"]);
    setMinTradeSize(j["MinTradeSize"]);
    // FIXME: setting Notice causes nullptr exception
//    setNotice(j["Notice"]);
}

Market::Market(Market &&o)
{
    currency = std::move(o.currency);
    currencyLong = std::move(o.currencyLong);
    marketName = std::move(o.marketName);
    isActive = std::move(o.isActive);
    minTradeSize = std::move(o.minTradeSize);
}

void Market::start()
{
    worker = std::thread(&Market::workerThread, this);
}

void Market::workerThread() noexcept
{
    std::locale loc;
    auto url = "https://bittrex.com/api/v1.1/public/getticker" + std::tolower(this->marketName, loc);

    while(true) {

    }

}
