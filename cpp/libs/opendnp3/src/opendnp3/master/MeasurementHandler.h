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
#ifndef OPENDNP3_MEASUREMENTHANDLER_H
#define OPENDNP3_MEASUREMENTHANDLER_H

#include <openpal/logging/Logger.h>

#include "opendnp3/master/ISOEHandler.h"
#include "opendnp3/app/parsing/APDUHandlerBase.h"
#include "opendnp3/app/parsing/IterableTransforms.h"
#include "opendnp3/app/APDUHeader.h"

namespace opendnp3
{

/**
 * Dedicated class for processing response data in the master.
 */
class MeasurementHandler : public APDUHandlerBase
{

public:

	/**
	* Static helper function for interpreting a response as a measurement response
	*/
	static bool ProcessMeasurements(const openpal::ReadBufferView& objects, openpal::Logger* pLogger, ISOEHandler* pHandler);	

	/**
	* Creates a new ResponseLoader instance.
	*
	* @param logger	the Logger that the loader should use for message reporting
	*/
	MeasurementHandler(const openpal::Logger& logger, ISOEHandler* pSOEHandler);	

	~MeasurementHandler();

private:

	static TimestampMode ModeFromType(GroupVariation gv);

	// Handle the CTO objects
	IINField ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group51Var1>&) override final;
	IINField ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group51Var2>&) override final;

	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)  final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas) override final;
	IINField ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas) override final;

	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas) override final;

	template <class T>
	IINField LoadAny(const HeaderRecord& record, TimestampMode tsmode, const IterableBuffer<IndexedValue<T, uint16_t>>& meas)
	{		
		this->CheckForTxStart();
		pSOEHandler->OnReceiveHeader(HeaderInfo(record.enumeration, record.GetQualifierCode(), tsmode), meas);
		return IINField();
	}

	template <class T>
	IINField ProcessWithCTO(const HeaderRecord& record, const IterableBuffer<IndexedValue<T, uint16_t>>& meas);


	bool txInitiated;
	ISOEHandler* pSOEHandler;

	TimestampMode ctoMode;
	uint32_t ctoHeader;
	uint64_t commonTimeOccurence;

	void CheckForTxStart();

};

template <class T>
IINField MeasurementHandler::ProcessWithCTO(const HeaderRecord& record, const IterableBuffer<IndexedValue<T, uint16_t>>& meas)
{	
	if (ctoMode != TimestampMode::INVALID && ((ctoHeader+1) == GetCurrentHeader()))
	{
		auto mode = this->ctoMode;
		auto timestamp = this->commonTimeOccurence;

		auto addTime = [timestamp](const IndexedValue<T, uint16_t>& value)
		{
			T copy(value.value);
			copy.time = DNPTime(copy.time + timestamp);
			return IndexedValue<T, uint16_t>(copy, value.index);
		};

		auto transform = MapIterableBuffer< IndexedValue<T, uint16_t>, IndexedValue<T, uint16_t> >(&meas, addTime);

		this->ctoMode = TimestampMode::INVALID;

		return this->LoadAny(record, mode, transform);
	}
	else
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "No prior CTO objects for %s", GroupVariationToString(record.enumeration));
		return IINField(IINBit::PARAM_ERROR);
	}
}

}



#endif

