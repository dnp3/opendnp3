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

	void Update(const Binary& arEvent, PointClass aClass, size_t aIndex) {
		BinaryInfo v(arEvent, aClass, aIndex);
		mBinaryEvents.push_back(v);
	}

	void Update(const Analog& arEvent, PointClass aClass, size_t aIndex) {
		AnalogInfo v(arEvent, aClass, aIndex);
		mAnalogEvents.push_back(v);
	}

	void Update(const Counter& arEvent, PointClass aClass, size_t aIndex) {
		CounterInfo v(arEvent, aClass, aIndex);
		mCounterEvents.push_back(v);
	}	

	std::deque<BinaryInfo> mBinaryEvents;
	std::deque<AnalogInfo> mAnalogEvents;
	std::deque<CounterInfo> mCounterEvents;
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

