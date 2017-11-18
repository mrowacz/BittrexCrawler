/*
 * Stock.cpp
 *
 *  Created on: 10 lis 2017
 *      Author: lukasz
 */

#include <chrono>
#include <thread>
#include <json.hpp>
#include <cpr/cpr.h>
#include <exception>

#include "Stock.h"

using json = nlohmann::json;

Stock::Stock() : quit_monitor({0}), sub()
{
}

Stock::~Stock()
{
    quit_monitor = 1;
	monitor.join();
    std::cout << "Quit monitor thread" << std::endl;
}

void Stock::start()
{
    monitor = std::thread([this]() {
        while(1) {
            auto r = cpr::Get(cpr::Url{"https://bittrex.com/api/v1.1/public/getmarkets"},
                              cpr::VerifySsl{false});
            auto j = json::parse(r.text);

            if (j.find("result") != j.end())
            {
                auto j_arr = *j.find("result");
                auto counter = 0;
                std::for_each(j_arr.begin(), j_arr.end(), [this, &counter](json it) -> void {
                    if (!it.at("BaseCurrency").get<std::string>().compare("BTC")) {
                        std::string marketStr = it["MarketName"];
                        auto m_entry = markets.find(marketStr);
                        if (m_entry == markets.end()) {
                            // FIXME: Add proper logging point
                            std::cout << "Added new Market " << marketStr << std::endl;
                            std::cout << it << std::endl;

                            try {
                            	markets.emplace(std::piecewise_construct,
                            			std::forward_as_tuple(marketStr),
										std::forward_as_tuple(it, this->sub));

                            	counter ++;
                            } catch (std::exception& e) {
								std::cout << e.what() << std::endl;
							}
                        } else {
                            std::cout << marketStr << " already exists" << std::endl;
                        }
                    }
                });
                std::cout << "Added new currencies " << counter << std::endl;
            }

            wait(10000, [this]() -> bool {
                if (this->quit_monitor == 1)
                    return true;
                return false;
            });
            if (this->quit_monitor == 1)
                break;
        }
    });
}

//TODO move this into separate library
void Stock::wait(unsigned int ms_time, std::function<bool()> cmp) {
    using chr = std::chrono::high_resolution_clock;

    auto end = chr::now() + std::chrono::milliseconds(ms_time);
    while(true) {
        if (chr::now() > end || cmp()) {
            break;
        }

        std::this_thread::yield();
    }
}
