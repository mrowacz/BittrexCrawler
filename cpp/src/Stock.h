/*
 * Stock.h
 *
 *  Created on: 10 lis 2017
 *      Author: lukasz
 */

#ifndef SRC_STOCK_H_
#define SRC_STOCK_H_

#include <map>
#include <set>
#include <atomic>
#include <thread>
#include <iostream>
#include <unordered_map>

#include "Market.h"

class Stock {
public:
	Stock();
	virtual ~Stock();
	void start();

private:
	void wait(unsigned int ms_time, std::function<bool()> cmp);

    std::unordered_map<std::string, Market> markets;
	std::atomic<unsigned> quit_monitor;
	std::thread monitor;
};

#endif /* SRC_STOCK_H_ */
