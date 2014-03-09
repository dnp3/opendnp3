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
#ifndef __MOCK_SOE_HANDLER_H_
#define __MOCK_SOE_HANDLER_H_

#include <opendnp3/master/ISOEHandler.h>
#include <opendnp3/app/OctetString.h>

#include <openpal/Location.h>

#include "Exception.h"

#include <map>


namespace opendnp3
{

// simple measurement handler for testing purposes
class MockSOEHandler : public ISOEHandler
{
public:

	MockSOEHandler()
	{}

	void LoadStatic(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<Binary, uint16_t>& value)
		{
			mBinaryMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<DoubleBitBinary, uint16_t>& value)
		{
			mDoubleBinaryMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<Analog, uint16_t>& value)
		{
			mAnalogMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<Counter, uint16_t>& value)
		{
			mCounterMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<FrozenCounter, uint16_t>& value)
		{
			mFrozenCounterMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<BinaryOutputStatus, uint16_t>& value)
		{
			mBinaryOutputStatusMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<AnalogOutputStatus, uint16_t>& value)
		{
			mAnalogOutputStatusMap[value.index] = value.value;
		});
	}

	void LoadStatic(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<OctetString, uint16_t>& value)
		{
			mOctetStringMap[value.index] = value.value;
		});
	}



	void LoadEvent(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<Binary, uint16_t>& value)
		{
			mEventBinaryMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<DoubleBitBinary, uint16_t>& value)
		{
			mEventDoubleBinaryMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<Analog, uint16_t>& value)
		{
			mEventAnalogMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<Counter, uint16_t>& value)
		{
			mCounterMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<FrozenCounter, uint16_t>& value)
		{
			mEventFrozenCounterMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<BinaryOutputStatus, uint16_t>& value)
		{
			mEventBinaryOutputStatusMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<AnalogOutputStatus, uint16_t>& value)
		{
			mAnalogOutputStatusMap[value.index] = value.value;
		});
	}

	void LoadEvent(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final
	{
		meas.foreach([this](const IndexedValue<OctetString, uint16_t>& value)
		{
			mEventOctetStringMap[value.index] = value.value;
		});
	}

	void Clear()
	{
		mBinaryMap.clear();
		mDoubleBinaryMap.clear();
		mAnalogMap.clear();
		mCounterMap.clear();
		mFrozenCounterMap.clear();
		mBinaryOutputStatusMap.clear();
		mAnalogOutputStatusMap.clear();
		mOctetStringMap.clear();

		mEventBinaryMap.clear();
		mEventDoubleBinaryMap.clear();
		mEventAnalogMap.clear();
		mEventCounterMap.clear();
		mEventFrozenCounterMap.clear();
		mEventBinaryOutputStatusMap.clear();
		mEventAnalogOutputStatusMap.clear();
		mEventOctetStringMap.clear();
	}

	Binary GetBinary(uint32_t aIndex)
	{
		return GetAny<Binary>(aIndex, mBinaryMap);
	}
	DoubleBitBinary GetDoubleBinary(uint32_t aIndex)
	{
		return GetAny<DoubleBitBinary>(aIndex, mDoubleBinaryMap);
	}
	Analog GetAnalog(uint32_t aIndex)
	{
		return GetAny<Analog>(aIndex, mAnalogMap);
	}
	Counter GetCounter(uint32_t aIndex)
	{
		return GetAny<Counter>(aIndex, mCounterMap);
	}
	FrozenCounter GetFrozenCounter(uint32_t aIndex)
	{
		return GetAny<FrozenCounter>(aIndex, mFrozenCounterMap);
	}
	BinaryOutputStatus GetBinaryOutputStatus(uint32_t aIndex)
	{
		return GetAny<BinaryOutputStatus>(aIndex, mBinaryOutputStatusMap);
	}
	AnalogOutputStatus GetAnalogOutputStatus(uint32_t aIndex)
	{
		return GetAny<AnalogOutputStatus>(aIndex, mAnalogOutputStatusMap);
	}
	OctetString GetOctetString(uint32_t aIndex)
	{
		return GetAny<OctetString>(aIndex, mOctetStringMap);
	}

	Binary GetEventBinary(uint32_t aIndex)
	{
		return GetAny<Binary>(aIndex, mEventBinaryMap);
	}
	DoubleBitBinary GetEventDoubleBinary(uint32_t aIndex)
	{
		return GetAny<DoubleBitBinary>(aIndex, mEventDoubleBinaryMap);
	}
	Analog GetEventAnalog(uint32_t aIndex)
	{
		return GetAny<Analog>(aIndex, mEventAnalogMap);
	}
	Counter GetEventCounter(uint32_t aIndex)
	{
		return GetAny<Counter>(aIndex, mEventCounterMap);
	}
	FrozenCounter GetEventFrozenCounter(uint32_t aIndex)
	{
		return GetAny<FrozenCounter>(aIndex, mEventFrozenCounterMap);
	}
	BinaryOutputStatus GetEventBinaryOutputStatus(uint32_t aIndex)
	{
		return GetAny<BinaryOutputStatus>(aIndex, mEventBinaryOutputStatusMap);
	}
	AnalogOutputStatus GetEventAnalogOutputStatus(uint32_t aIndex)
	{
		return GetAny<AnalogOutputStatus>(aIndex, mEventAnalogOutputStatusMap);
	}
	OctetString GetEventOctetString(uint32_t aIndex)
	{
		return GetAny<OctetString>(aIndex, mEventOctetStringMap);
	}

protected:

	void Start() {}
	void End() {}


private:

	template <typename T>
	struct PointMap
	{
		typedef std::map<uint32_t, T> Type;
	};

	template <class T>
	T GetAny(uint32_t aIndex, const typename PointMap<T>::Type& arMap)
	{
		auto iter = arMap.find(aIndex);
		if(iter == arMap.end()) throw Exception(LOCATION, "Index not found in map");
		else return iter->second;
	}

	PointMap<Binary>::Type mBinaryMap;
	PointMap<DoubleBitBinary>::Type mDoubleBinaryMap;
	PointMap<Analog>::Type mAnalogMap;
	PointMap<Counter>::Type mCounterMap;
	PointMap<FrozenCounter>::Type mFrozenCounterMap;
	PointMap<BinaryOutputStatus>::Type mBinaryOutputStatusMap;
	PointMap<AnalogOutputStatus>::Type mAnalogOutputStatusMap;
	PointMap<OctetString>::Type mOctetStringMap;

	PointMap<Binary>::Type mEventBinaryMap;
	PointMap<DoubleBitBinary>::Type mEventDoubleBinaryMap;
	PointMap<Analog>::Type mEventAnalogMap;
	PointMap<Counter>::Type mEventCounterMap;
	PointMap<FrozenCounter>::Type mEventFrozenCounterMap;
	PointMap<BinaryOutputStatus>::Type mEventBinaryOutputStatusMap;
	PointMap<AnalogOutputStatus>::Type mEventAnalogOutputStatusMap;
	PointMap<OctetString>::Type mEventOctetStringMap;

};

}

#endif

