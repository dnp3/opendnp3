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
#ifndef OPENDNP3_APDUHANDLERBASE_H
#define OPENDNP3_APDUHANDLERBASE_H

#include "opendnp3/app/IINField.h"
#include "opendnp3/app/IAPDUHandler.h"
#include "opendnp3/app/IterableTransforms.h"

#include "opendnp3/gen/TimestampMode.h"

#include "opendnp3/LogLevels.h"
#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>



namespace opendnp3
{

/**
 * Base class used to handle APDU object headers
 */
class APDUHandlerBase : public IAPDUHandler
{
public:

	/**
	 * @param arLogger	the Logger that the loader should use for message reporting
	 */
	APDUHandlerBase(const openpal::Logger& logger);

	uint32_t NumIgnoredHeaders() const
	{
		return ignoredHeaders;
	}

	IINField Errors() const;

	virtual void AllObjects(const HeaderRecord& record) override final;

	virtual void OnRangeRequest(const HeaderRecord& record, const Range& range) override final;

	virtual void OnCountRequest(const HeaderRecord& record, uint32_t count) override final;

	virtual void OnIIN(const HeaderRecord& record, const IterableBuffer<IndexedValue<bool, uint16_t>>& meas) override final;

	virtual void OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group50Var1>&) override final;

	virtual void OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var1>&) override final;
	virtual void OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var2>&) override final;

	virtual void OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group52Var2>&) override final;

	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;

	// events - 16bit indices

	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;

	// events - 8bit indices

	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint8_t>>& meas) override final;

	// commands - 16bit indices

	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) override final;

	// commands - 8bit indices

	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas) override final;


protected:

	void Reset();
	TimestampMode GetCTO(uint64_t& cto);

	inline uint32_t GetCurrentHeader()
	{
		return currentHeader;
	}

	inline bool IsFirstHeader()
	{
		return currentHeader == 0;
	}


	virtual void _AllObjects(const HeaderRecord& record);

	virtual void _OnRangeRequest(const HeaderRecord& record, const Range& range);

	virtual void _OnCountRequest(const HeaderRecord& record, uint32_t count);

	virtual void _OnIIN(const HeaderRecord& record, const IterableBuffer<IndexedValue<bool, uint16_t>>& meas);

	virtual void _OnCountOf(const HeaderRecord& record, const IterableBuffer<Group50Var1>& objects);
	virtual void _OnCountOf(const HeaderRecord& record, const IterableBuffer<Group52Var2>&);

	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas);
	virtual void _OnRange(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas);

	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas);

	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas);

	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas);
	virtual void _OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas);

protected:

	openpal::Logger logger;
	uint32_t ignoredHeaders;
	IINField errors;

private:

	static TimestampMode ModeFromType(GroupVariation gv);

	uint64_t cto;
	uint32_t ctoHeader;
	TimestampMode ctoMode;

	uint32_t currentHeader;

	template <class T>
	static IndexedValue<T, uint16_t> Widen(const IndexedValue<T, uint8_t> value)
	{
		return value.template Widen<uint16_t>();
	}

	template <class T>
	void OnIndexPrefixCTO(const HeaderRecord& record, const IterableBuffer<IndexedValue<T, uint16_t>>& meas);
};

template <class T>
void APDUHandlerBase::OnIndexPrefixCTO(const HeaderRecord& record, const IterableBuffer<IndexedValue<T, uint16_t>>& meas)
{
	uint64_t commonTime;
	auto mode = GetCTO(commonTime);
	if (mode != TimestampMode::INVALID)
	{
		auto transform = MapIterableBuffer< IndexedValue<T, uint16_t>, IndexedValue<T, uint16_t> >(&meas,

		                 [commonTime](const IndexedValue<T, uint16_t>& value)
		{
			T copy(value.value);
			copy.time += commonTime;
			return IndexedValue<T, uint16_t>(copy, value.index);
		}

		                                                                                          );
		this->_OnIndexPrefix(record, mode, transform);
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Received CTO objects without preceding common time, discarding");		
	}
}



}



#endif

