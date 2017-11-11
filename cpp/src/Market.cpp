//
// Created by lukasz on 11.11.17.
//

#include "Market.h"

#include <iostream>

Market::Market(nlohmann::json j)
{
    std::cout << j << std::endl;
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

unsigned int Market::getMinConfirmation() const {
    return minConfirmation;
}

void Market::setMinConfirmation(unsigned int minConfirmation) {
    Market::minConfirmation = minConfirmation;
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
