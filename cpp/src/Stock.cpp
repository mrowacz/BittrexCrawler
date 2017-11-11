/*
 * Stock.cpp
 *
 *  Created on: 10 lis 2017
 *      Author: lukasz
 */

#include <chrono>
#include <cpr/cpr.h>

#include "Stock.h"

Stock::Stock() : quit_monitor({0})
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
            auto r = cpr::Get(cpr::Url{"https://bittrex.com/api/v1.1/public/getcurrencies"},
                              cpr::VerifySsl{false});
            std::cout << r.status_code << " " << r.text << std::endl;

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