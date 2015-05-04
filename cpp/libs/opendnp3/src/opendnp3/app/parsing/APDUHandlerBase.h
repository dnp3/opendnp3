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

#include "opendnp3/app/parsing/IAPDUHandler.h"
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
	 * @param logger the Logger that the loader should use for message reporting
	 */
	APDUHandlerBase(openpal::Logger logger);

	uint32_t NumIgnoredHeaders() const
	{
		return numIgnoredHeaders;
	}

	IINField Errors() const;

	virtual void AllObjects(const HeaderRecord& record) override final;

	virtual void OnRangeRequest(const HeaderRecord& record, const Range& range) override final;

	virtual void OnCountRequest(const HeaderRecord& record, uint16_t count) override final;

	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var1& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var2& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var5& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var6& value) override final;

	virtual void OnCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group50Var1& value) override final;
	virtual void OnCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var1& value) override final;
	virtual void OnCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var2& value) override final;
	virtual void OnCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group52Var2& value) override final;
	virtual void OnCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group120Var4& value) override final;

	virtual void OnRange(const HeaderRecord& record, uint16_t index, uint32_t count, const IINValue& value) override final;

	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas) override final;	
	virtual void OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group121Var1, uint16_t>>& meas) override final;

	// events

	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas) override final;

	// security stat events 
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group122Var1, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group122Var2, uint16_t>>& meas) override final;

	// commands

	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) override final;
	virtual void OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) override final;	

protected:

	void Reset();	

	inline uint32_t GetCurrentHeader()
	{
		return numTotalHeaders;
	}

	inline bool IsFirstHeader()
	{
		return numTotalHeaders == 0;
	}


	virtual IINField ProcessAllObjects(const HeaderRecord& record);
	virtual IINField ProcessRangeRequest(const HeaderRecord& record, const Range& range);
	virtual IINField ProcessCountRequest(const HeaderRecord& record, uint16_t count);

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var5& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value);

	virtual IINField ProcessIIN(const HeaderRecord& record, uint16_t index, uint32_t count, const IINValue& value);

	virtual IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group50Var1& value);
	virtual IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var1& value);
	virtual IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var2& value);
	virtual IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group52Var2& value);
	virtual IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group120Var4& value);
	
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas);
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas);	
	virtual IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group121Var1, uint16_t>>& meas);

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas);

	// security stat events 
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group122Var1, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group122Var2, uint16_t>>& meas);

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas);
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas);	

protected:

	openpal::Logger logger;	

	// overridable to receive events for every header
	virtual void OnHeaderResult(const HeaderRecord& record, const IINField& result) {}
	
private:

	inline void Record(const HeaderRecord& record, const IINField& result)
	{
		errors |= result;
		++numTotalHeaders;
		this->OnHeaderResult(record, result);
	}

	inline IINField ProcessUnsupportedHeader()
	{
		++numIgnoredHeaders;
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}

	IINField errors;
	uint32_t numTotalHeaders;
	uint32_t numIgnoredHeaders;			
};

}



#endif

