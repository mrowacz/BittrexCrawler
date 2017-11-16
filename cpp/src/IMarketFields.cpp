//
// Created by lukasz on 13.11.17.
//

#include "IMarketFields.h"
#include <iostream>
#include <algorithm>

const std::string &IMarketFields::getCurrency() const {
    return currency;
}

void IMarketFields::setCurrency(const std::string &currency) {
    IMarketFields::currency = currency;
}

const std::string &IMarketFields::getCurrencyLong() const {
    return currencyLong;
}

void IMarketFields::setCurrencyLong(const std::string &currencyLong) {
    IMarketFields::currencyLong = currencyLong;
}

float IMarketFields::getTxFee() const {
    return txFee;
}

void IMarketFields::setTxFee(float txFee) {
    IMarketFields::txFee = txFee;
}

bool IMarketFields::isIsActive() const {
    return isActive;
}

void IMarketFields::setIsActive(bool isActive) {
    IMarketFields::isActive = isActive;
}

const std::string &IMarketFields::getCoinType() const {
    return coinType;
}

void IMarketFields::setCoinType(const std::string &coinType) {
    IMarketFields::coinType = coinType;
}

const std::string &IMarketFields::getBaseAddress() const {
    return baseAddress;
}

void IMarketFields::setBaseAddress(const std::string &baseAddress) {
    IMarketFields::baseAddress = baseAddress;
}

const std::string &IMarketFields::getNotice() const {
    return notice;
}

void IMarketFields::setNotice(const std::string &notice) {
    IMarketFields::notice = notice;
}

const std::string &IMarketFields::getMarketName() const {
    return marketName;
}

void IMarketFields::setMarketName(const std::string &marketName) {
	this->marketName = std::string(marketName);
	std::transform(marketName.begin(), marketName.end(), IMarketFields::marketName.begin(), ::tolower);
}

unsigned int IMarketFields::getMinTradeSize() const {
    return minTradeSize;
}

void IMarketFields::setMinTradeSize(unsigned int minTradeSize) {
    IMarketFields::minTradeSize = minTradeSize;
}
