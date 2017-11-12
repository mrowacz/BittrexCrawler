//
// Created by lukasz on 11.11.17.
//

#ifndef BITTREXCRAWLER_MARKET_H
#define BITTREXCRAWLER_MARKET_H

#include <thread>
#include <string>
#include <json.hpp>

class Market {

public:
    Market(nlohmann::json j);
    Market(Market &&o);
    Market(const Market &that) = delete;
    Market& operator=(const Market &rhs) = delete;

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
    const std::string &getMarketName() const;
    void setMarketName(const std::string &marketName);
    unsigned int getMinTradeSize() const;
    void setMinTradeSize(unsigned int minTradeSize);

    void start();

private:

    void workerThread() noexcept;

    std::string currency;
    std::string currencyLong;
    std::string marketName;
    unsigned int minTradeSize;
    float txFee;
    bool isActive;
    std::string coinType;
    std::string baseAddress;
    std::string notice;

    std::thread worker;
};


#endif //BITTREXCRAWLER_MARKET_H
