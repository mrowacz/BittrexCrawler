/*
 * Subscriber.cpp
 *
 *  Created on: Nov 17, 2017
 *      Author: mrowacz
 */

#include "Subscriber.h"

Subscriber::Subscriber() {}

Subscriber::~Subscriber() {}

void Subscriber::enqueue(MongoEntry&& e)
{
	std::lock_guard<std::mutex> lock(m);
	dq.emplace_back(std::move(e));
}
