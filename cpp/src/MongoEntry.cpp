/*
 * MongoEntry.cpp
 *
 *  Created on: Nov 17, 2017
 *      Author: mrowacz
 */

#include "MongoEntry.h"

MongoEntry::MongoEntry(const std::string& market, float bid, float ask,
		float last) :
			market(market),
			bid(bid),
			ask(ask),
			last(last)
{
}

MongoEntry::~MongoEntry() {
	// TODO Auto-generated destructor stub
}

