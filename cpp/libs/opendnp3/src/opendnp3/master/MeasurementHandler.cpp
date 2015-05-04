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
#include "MeasurementHandler.h"

#include "opendnp3/app/parsing/APDUParser.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

ParseResult MeasurementHandler::ProcessMeasurements(const openpal::ReadBufferView& objects, openpal::Logger& logger, ISOEHandler* pHandler)
{
	MeasurementHandler handler(logger, pHandler);
	return APDUParser::ParseAll(objects, handler, &logger);	
}

MeasurementHandler::MeasurementHandler(const openpal::Logger& logger, ISOEHandler* pSOEHandler_) : 
	APDUHandlerBase(logger),
	txInitiated(false),
	pSOEHandler(pSOEHandler_),
	ctoMode(TimestampMode::INVALID),
	ctoHeader(0),
	commonTimeOccurence(0)
{
	
}

MeasurementHandler::~MeasurementHandler()
{
	if (txInitiated)
	{
		Transaction::End(pSOEHandler);
	}
}

TimestampMode MeasurementHandler::ModeFromType(GroupVariation gv)
{
	return TypeHasTimestamp(gv) ? TimestampMode::SYNCHRONIZED : TimestampMode::INVALID;
}


void MeasurementHandler::CheckForTxStart()
{
	if (!txInitiated)
	{
		txInitiated = true;
		Transaction::Start(pSOEHandler);
	}
}

IINField MeasurementHandler::ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var1& cto)
{	
	if (pos == 0 && total == 1)
	{
		ctoMode = TimestampMode::SYNCHRONIZED;
		commonTimeOccurence = cto.time;
		ctoHeader = this->GetCurrentHeader();		
	}
	return IINField::Empty();
}

IINField MeasurementHandler::ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var2& cto)
{	
	if (pos == 0 && total == 1)
	{
		ctoMode = TimestampMode::UNSYNCHRONIZED;
		commonTimeOccurence = cto.time;
		ctoHeader = this->GetCurrentHeader();
	}

	return IINField::Empty();
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group121Var1, uint16_t>>& meas)
{
	auto convert = [](const IndexedValue<Group121Var1, uint16_t>& pair)
	{
		SecurityStat ss(pair.value.flags, pair.value.assocId, pair.value.value);		
		return IndexedValue<SecurityStat, uint16_t>(ss, pair.index);
	};

	auto transformed = MapIterableBuffer< IndexedValue<Group121Var1, uint16_t>, IndexedValue<SecurityStat, uint16_t> >(&meas, convert);

	return this->LoadAny(record, ModeFromType(record.enumeration), transformed);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	if (record.enumeration == GroupVariation::Group2Var3)
	{
		return this->ProcessWithCTO(record, meas);
	}
	else
	{
		return this->LoadAny(record, ModeFromType(record.enumeration), meas);
	}
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	if (record.enumeration == GroupVariation::Group4Var3)
	{
		return this->ProcessWithCTO(record, meas);
	}
	else
	{
		return this->LoadAny(record, ModeFromType(record.enumeration), meas);
	}
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& meas)
{
	return this->LoadAny(record, ModeFromType(record.enumeration), meas);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group122Var1, uint16_t>>& meas)
{
	auto convert = [](const IndexedValue<Group122Var1, uint16_t>& pair)
	{
		SecurityStat ss(pair.value.flags, pair.value.assocId, pair.value.value);
		return IndexedValue<SecurityStat, uint16_t>(ss, pair.index);
	};

	auto transformed = MapIterableBuffer< IndexedValue<Group122Var1, uint16_t>, IndexedValue<SecurityStat, uint16_t> >(&meas, convert);

	return this->LoadAny(record, ModeFromType(record.enumeration), transformed);
}

IINField MeasurementHandler::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Group122Var2, uint16_t>>& meas)
{
	auto convert = [](const IndexedValue<Group122Var2, uint16_t>& pair)
	{
		SecurityStat ss(pair.value.flags, pair.value.assocId, pair.value.value, pair.value.time);
		return IndexedValue<SecurityStat, uint16_t>(ss, pair.index);
	};

	auto transformed = MapIterableBuffer< IndexedValue<Group122Var2, uint16_t>, IndexedValue<SecurityStat, uint16_t> >(&meas, convert);

	return this->LoadAny(record, ModeFromType(record.enumeration), transformed);
}

}


