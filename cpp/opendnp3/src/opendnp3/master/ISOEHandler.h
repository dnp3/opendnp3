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
#ifndef __I_SOE_HANDLER_H_
#define	__I_SOE_HANDLER_H_

#include <openpal/ITransactable.h>
#include "opendnp3/app/IterableBuffer.h"
#include "opendnp3/app/IndexedValue.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/app/OctetString.h"

namespace opendnp3
{

/**
* An interface for Sequence-Of-Events (SOE) callbacks from a master stack to
* the application layer.
*
* A call is made to the appropriate member method for every header in a APDU
*
*/
class ISOEHandler : public openpal::ITransactable
{
public:

	virtual void LoadStatic(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) = 0;
	virtual void LoadStatic(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) = 0;

	virtual void LoadEvent(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) = 0;
	virtual void LoadEvent(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) = 0;

	virtual ~ISOEHandler() {}
};


class NullSOEHandler : public ISOEHandler
{

public:

	static ISOEHandler* Inst()
	{
		return &msInstance;
	}

	void LoadStatic(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final {}
	void LoadStatic(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final {}

	void LoadEvent(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final {}
	void LoadEvent(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final {}

protected:

	void Start() override final {}
	void End() override final {}

private:
	NullSOEHandler()
	{}

	static NullSOEHandler msInstance;
};

/*
class PrintingSOEHandler : public ISOEHandler
{

public:
	static ISOEHandler* Inst()
	{
		return &msInstance;
	}

	void LoadStatic(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	void LoadStatic(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;

	void LoadEvent(const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	void LoadEvent(const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;

protected:

	void Start() final {}
	void End() final {}

private:

	template <class T>
	static void Print(const IterableBuffer<IndexedValue<T, uint16_t>>& buffer, const std::string& name)
	{
		buffer.foreach([&](const IndexedValue<T, uint16_t>& pair)
		{

			std::cout << name << " [" << pair.index << "] : " <<
			          ValueToString(pair.value) << " : " <<
			          static_cast<int>(pair.value.GetQuality()) << " : " <<
			          GetTimeString(pair.value) <<
			          std::endl;
		});
	}

	template <class T>
	static std::string ValueToString(const T& meas)
	{
		std::ostringstream oss;
		oss << meas.GetValue();
		return oss.str();
	}

	static std::string GetTimeString(const Measurement& meas)
	{
		std::ostringstream oss;
		if (meas.IsTimeValid())
		{
			oss << meas.GetTime();
		}
		else
		{
			oss << "no timestamp";
		}
		return oss.str();
	}

	static std::string ValueToString(const DoubleBitBinary& meas)
	{
		return DoubleBitToString(meas.GetValue());
	}

	PrintingSOEHandler()
	{}

	static PrintingSOEHandler msInstance;
};
*/


}

#endif
