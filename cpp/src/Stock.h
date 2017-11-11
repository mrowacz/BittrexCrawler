/*
 * Stock.h
 *
 *  Created on: 10 lis 2017
 *      Author: lukasz
 */

#ifndef SRC_STOCK_H_
#define SRC_STOCK_H_

#include <map>
#include <atomic>
#include <thread>
#include <iostream>

class Stock {
public:
	Stock();
	virtual ~Stock();
	void start();
private:

	void wait(unsigned int ms_time, std::function<bool()> cmp);
	std::atomic<unsigned> quit_monitor;
	std::thread monitor;
};

#endif /* SRC_STOCK_H_ */
