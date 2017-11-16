//
// Created by lukasz on 11.11.17.
//

#ifndef BITTREXCRAWLER_MARKET_H
#define BITTREXCRAWLER_MARKET_H

#include <thread>
#include <string>
#include <json.hpp>

#include "IMarketFields.h"

class Market : public IMarketFields {

public:
    Market(nlohmann::json j);
    Market(Market &&o) = delete;
    Market(const Market &that) = delete;
    Market& operator=(const Market &rhs) = delete;

    virtual ~Market();

private:
    void workerThread();

    std::thread worker;
};


#endif //BITTREXCRAWLER_MARKET_H
