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

const std::string &Market::getCurrency() const {
    return currency;
}

void Market::setCurrency(const std::string &currency) {
    Market::currency = currency;
}

const std::string &Market::getCurrencyLong() const {
    return currencyLong;
}

void Market::setCurrencyLong(const std::string &currencyLong) {
    Market::currencyLong = currencyLong;
}

float Market::getTxFee() const {
    return txFee;
}

void Market::setTxFee(float txFee) {
    Market::txFee = txFee;
}

bool Market::isIsActive() const {
    return isActive;
}

void Market::setIsActive(bool isActive) {
    Market::isActive = isActive;
}

const std::string &Market::getCoinType() const {
    return coinType;
}

void Market::setCoinType(const std::string &coinType) {
    Market::coinType = coinType;
}

const std::string &Market::getBaseAddress() const {
    return baseAddress;
}

void Market::setBaseAddress(const std::string &baseAddress) {
    Market::baseAddress = baseAddress;
}

const std::string &Market::getNotice() const {
    return notice;
}

void Market::setNotice(const std::string &notice) {
    Market::notice = notice;
}

const std::string &Market::getMarketName() const {
    return marketName;
}

void Market::setMarketName(const std::string &marketName) {
    Market::marketName = marketName;
}

unsigned int Market::getMinTradeSize() const {
    return minTradeSize;
}

void Market::setMinTradeSize(unsigned int minTradeSize) {
    Market::minTradeSize = minTradeSize;
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
