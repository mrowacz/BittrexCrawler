//
// Created by lukasz on 11.11.17.
//

#include "Market.h"

#include <chrono>
#include <thread>
#include <locale>
#include <iostream>
#include <sstream>
#include <cpr/cpr.h>

using json = nlohmann::json;

Market::Market(nlohmann::json j)
{
    setCurrency(j["MarketCurrency"]);
    setCurrencyLong(j["MarketCurrencyLong"]);
    setMarketName(j["MarketName"]);
    setIsActive(j["IsActive"]);
    setMinTradeSize(j["MinTradeSize"]);
    // FIXME: setting Notice causes nullptr exception
    // setNotice(j["Notice"]);

    worker = std::thread(&Market::workerThread, this);
}

Market::~Market() {
	if (worker.joinable())
		worker.join();
}

void Market::workerThread()
{
    using namespace std::chrono_literals;

    std::locale loc;
    std::stringstream ss;

    ss << "https://bittrex.com/api/v1.1/public/getticker?market=";
    ss << this->marketName;
    std::string url = ss.str();

    while(true) {
    	{
    		auto r = cpr::Get(cpr::Url{url},
    				cpr::VerifySsl{false});
    		auto j = json::parse(r.text);

    		std::cout << j << std::endl;

    	    std::this_thread::sleep_for(2s);
    	}
    }

}
