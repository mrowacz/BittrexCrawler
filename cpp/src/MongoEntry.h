/*
 * MongoEntry.h
 *
 *  Created on: Nov 17, 2017
 *      Author: mrowacz
 */

#ifndef MONGOENTRY_H_
#define MONGOENTRY_H_

#include <string>

class MongoEntry {
public:
	MongoEntry() = delete;
	MongoEntry(const std::string& market, float bid, float ask, float last);

	MongoEntry(const MongoEntry &other) = delete;
	MongoEntry& operator=(const MongoEntry &other) = delete;
	MongoEntry& operator=(MongoEntry &&other) = delete;

	MongoEntry(MongoEntry &&other) = default;

	virtual ~MongoEntry();

private:
	std::string market;
	float bid;
	float ask;
	float last;
};

#endif /* MONGOENTRY_H_ */
