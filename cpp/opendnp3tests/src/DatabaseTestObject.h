/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __DATABASE_TEST_OBJECT_H_
#define __DATABASE_TEST_OBJECT_H_

#include <queue>
#include <opendnp3/Database.h>
#include <asiopal/Log.h>

namespace opendnp3
{

class MockEventBuffer : public IEventBuffer
{
public:

	virtual ~MockEventBuffer() {}

	void Update(const Binary& aValue, PointClass aClass, uint32_t aIndex) {
		PointInfo<Binary> v(aValue, aIndex, aClass);
		mBinaryEvents.push_back(v);
	}

	void Update(const Analog& aValue, PointClass aClass, uint32_t aIndex) {
		PointInfo<Analog> v(aValue, aIndex, aClass);
		mAnalogEvents.push_back(v);
	}

	void Update(const Counter& aValue, PointClass aClass, uint32_t aIndex) {
		PointInfo<Counter> v(aValue, aIndex, aClass);
		mCounterEvents.push_back(v);
	}	

	std::deque<PointInfo<Binary>> mBinaryEvents;
	std::deque<PointInfo<Analog>> mAnalogEvents;
	std::deque<PointInfo<Counter>> mCounterEvents;
};

class DatabaseTestObject
{
public:
	DatabaseTestObject(openpal::LogLevel aLevel = openpal::LogLevel::Info) :
		db(openpal::Logger(&log, aLevel, "test")) {
		db.SetEventBuffer(&buffer);
	}

	asiopal::EventLog log;
	MockEventBuffer buffer;
	Database db;
};

}

#endif

