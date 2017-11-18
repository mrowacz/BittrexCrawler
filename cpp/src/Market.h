//
// Created by lukasz on 11.11.17.
//

#ifndef BITTREXCRAWLER_MARKET_H
#define BITTREXCRAWLER_MARKET_H

#include <thread>
#include <string>
#include <memory>
#include <json.hpp>

#include "IMarketFields.h"

class Subscriber;
class Market : public IMarketFields {

public:
    Market(nlohmann::json j, std::shared_ptr<Subscriber> &s);

    Market(Market &&o) = delete;
    Market(const Market &that) = delete;
    Market& operator=(const Market &rhs) = delete;

    virtual ~Market();

private:
    void workerThread();

    long success;
    long fail;

    std::thread worker;
    std::weak_ptr<Subscriber> subscriber;
};


#endif //BITTREXCRAWLER_MARKET_H
