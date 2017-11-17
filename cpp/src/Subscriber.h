/*
 * Subscriber.h
 *
 *  Created on: Nov 17, 2017
 *      Author: mrowacz
 */

#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include <iostream>
#include <mutex>
#include <deque>

#include "MongoEntry.h"

class Subscriber {
public:
	Subscriber();
	virtual ~Subscriber();

	void enqueue(MongoEntry&& e);

private:
	std::deque<MongoEntry> dq;
	std::mutex m;
};

#endif /* SUBSCRIBER_H_ */
