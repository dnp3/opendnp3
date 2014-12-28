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

#include <map>


namespace opendnp3
{

// simple measurement handler for testing purposes
class MockSOEHandler : public ISOEHandler
{
public:

	template <class T>
	class Record
	{
		public:

			Record() : sequence(0)
			{}
		
			Record(const T& meas_, const HeaderInfo& info_, uint32_t sequence_) :
				meas(meas_),				
				info(info_),
				sequence(sequence_)
			{}
				
	
			T meas;			
			HeaderInfo info;			
			uint32_t sequence;
	};

	MockSOEHandler() : soeCount(0)
	{}

	uint32_t TotalReceived() const
	{
		return soeCount;
	}	

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, binarySOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, doubleBinarySOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, analogSOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, counterSOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, frozenCounterSOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, binaryOutputStatusSOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, analogOutputStatusSOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, octetStringSOE);
	}

	void OnReceiveHeader(const HeaderInfo& info, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas) override final
	{
		this->RecordAny(info, meas, timeAndIntervalSOE);
	}



	void Clear()
	{
		soeCount = 0;

		binarySOE.clear();
		doubleBinarySOE.clear();
		analogSOE.clear();
		counterSOE.clear();
		frozenCounterSOE.clear();
		binaryOutputStatusSOE.clear();
		analogOutputStatusSOE.clear();
		octetStringSOE.clear();
	}	

	std::map<uint16_t, Record<Binary>> binarySOE;
	std::map<uint16_t, Record<DoubleBitBinary>> doubleBinarySOE;
	std::map<uint16_t, Record<Analog>> analogSOE;
	std::map<uint16_t, Record<Counter>> counterSOE;
	std::map<uint16_t, Record<FrozenCounter>> frozenCounterSOE;
	std::map<uint16_t, Record<BinaryOutputStatus>> binaryOutputStatusSOE;
	std::map<uint16_t, Record<AnalogOutputStatus>> analogOutputStatusSOE;
	std::map<uint16_t, Record<OctetString>> octetStringSOE;
	std::map<uint16_t, Record<TimeAndInterval>> timeAndIntervalSOE;

protected:

	void Start() {}
	void End() {}

private:

	uint32_t soeCount;

	template <class T>
	void RecordAny(const HeaderInfo& info, const IterableBuffer<IndexedValue<T, uint16_t>>& meas, std::map<uint16_t, Record<T> >& records)
	{
		meas.foreach([this, &info, &records](const IndexedValue<T, uint16_t>& value)
		{
			Record<T> record(value.value, info, soeCount);
			records[value.index] = record;
			++soeCount;
		});
	}


	

};

}

#endif

