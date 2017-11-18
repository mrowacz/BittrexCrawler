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

#include "Subscriber.h"

using json = nlohmann::json;

Market::Market(nlohmann::json j, std::shared_ptr<Subscriber> &s) : subscriber(s),
		success(0),
		fail(0)
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
    ss << marketName;
    std::string url = ss.str();

    while(true) {
    	{
    		auto r = cpr::Get(cpr::Url{url},
    				cpr::VerifySsl{false});
    		auto j = json::parse(r.text);

    		auto result = j["result"];
    		try {
				float bid = result["Bid"];
				float ask = result["Ask"];
				float last = result["Last"];

				if (auto ss = subscriber.lock())
					ss->enqueue(MongoEntry(marketName, bid, ask, last));

				success++;
    		} catch (std::exception& e) {
    			fail++;
    		}

    	    std::this_thread::sleep_for(1s);
    	}
    }

}
