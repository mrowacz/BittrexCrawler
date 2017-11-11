//
// Created by lukasz on 11.11.17.
//

#ifndef BITTREXCRAWLER_MARKET_H
#define BITTREXCRAWLER_MARKET_H

#include <string>
#include <json.hpp>

class Market {

public:
    Market(nlohmann::json j);

    // getters and setters
    const std::string &getCurrency() const;
    void setCurrency(const std::string &currency);
    const std::string &getCurrencyLong() const;
    void setCurrencyLong(const std::string &currencyLong);
    unsigned int getMinConfirmation() const;
    void setMinConfirmation(unsigned int minConfirmation);
    float getTxFee() const;
    void setTxFee(float txFee);
    bool isIsActive() const;
    void setIsActive(bool isActive);
    const std::string &getCoinType() const;
    void setCoinType(const std::string &coinType);
    const std::string &getBaseAddress() const;
    void setBaseAddress(const std::string &baseAddress);
    const std::string &getNotice() const;
    void setNotice(const std::string &notice);

private:
    std::string currency;
    std::string currencyLong;
    unsigned int minConfirmation;
    float txFee;
    bool isActive;
    std::string coinType;
    std::string baseAddress;
    std::string notice;
};


#endif //BITTREXCRAWLER_MARKET_H
