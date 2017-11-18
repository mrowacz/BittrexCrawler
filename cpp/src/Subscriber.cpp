/*
 * Subscriber.cpp
 *
 *  Created on: Nov 17, 2017
 *      Author: mrowacz
 */

#include <chrono>

#include "Subscriber.h"

Subscriber::Subscriber()
{
    using namespace std::chrono_literals;

	worker = std::thread([this]() {
		while(true)
		{
			{
				std::lock_guard<std::mutex> lock(m);
				dq.pop_front();
			}

    	    std::this_thread::sleep_for(1s);

		}
	});
}

Subscriber::~Subscriber() {}

void Subscriber::enqueue(MongoEntry&& e)
{
	std::lock_guard<std::mutex> lock(m);
	dq.emplace_back(std::move(e));
}
