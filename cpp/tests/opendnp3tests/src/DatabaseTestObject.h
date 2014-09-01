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

#include <opendnp3/outstation/Event.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>


namespace opendnp3
{

class MockEventBuffer : public IEventBuffer
{
public:

	void Update(const Event<Binary>& aEvent) override final
	{
		binaryEvents.push_back(aEvent);
	}

	void Update(const Event<DoubleBitBinary>& aEvent) override final
	{
		doubleBinaryEvents.push_back(aEvent);
	}

	void Update(const Event<Analog>& aEvent) override final
	{
		analogEvents.push_back(aEvent);
	}

	void Update(const Event<Counter>& aEvent) override final
	{
		counterEvents.push_back(aEvent);
	}

	void Update(const Event<FrozenCounter>& aEvent) override final
	{
		frozenCounterEvents.push_back(aEvent);
	}

	void Update(const Event<BinaryOutputStatus>& aEvent) override final
	{
		binaryOutputStatusEvents.push_back(aEvent);
	}

	void Update(const Event<AnalogOutputStatus>& aEvent) override final
	{
		analogOutputStatusEvents.push_back(aEvent);
	}

	std::deque<Event<Binary>> binaryEvents;
	std::deque<Event<DoubleBitBinary>> doubleBinaryEvents;
	std::deque<Event<Analog>> analogEvents;
	std::deque<Event<Counter>> counterEvents;
	std::deque<Event<FrozenCounter>> frozenCounterEvents;
	std::deque<Event<BinaryOutputStatus>> binaryOutputStatusEvents;
	std::deque<Event<AnalogOutputStatus>> analogOutputStatusEvents;
};

class DatabaseTestObject
{
public:
	DatabaseTestObject(const DatabaseTemplate& dbTemplate) :
		buffers(dbTemplate),
		db(buffers.GetFacade())
	{
		db.SetEventBuffer(buffer);
	}

private:
	DynamicallyAllocatedDatabase buffers;

public:
	MockEventBuffer buffer;
	Database db;
};

}

#endif

