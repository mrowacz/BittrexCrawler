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

#include "Stock.h"
#include "Market.h"

using json = nlohmann::json;

Stock::Stock() : quit_monitor({0})
{
}

Stock::~Stock()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

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
                for (auto it = j_arr.begin(); it != j_arr.end(); ++it) {
                    Market *m = new Market(*it);
                }

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