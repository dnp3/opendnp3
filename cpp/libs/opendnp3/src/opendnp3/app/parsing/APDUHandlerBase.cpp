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
#include "APDUHandlerBase.h"

using namespace openpal;

namespace opendnp3
{

APDUHandlerBase::APDUHandlerBase(openpal::Logger logger_) :
	logger(logger_),
	numTotalHeaders(0),
	numIgnoredHeaders(0)	
{

}

void APDUHandlerBase::Reset()
{
	numTotalHeaders = 0;
	numIgnoredHeaders = 0;	
	errors.Clear();	
}

IINField APDUHandlerBase::Errors() const
{
	return errors;
}

void APDUHandlerBase::AllObjects(const HeaderRecord& record)
{
	Record(this->ProcessAllObjects(record));	
}

void APDUHandlerBase::OnRangeRequest(const HeaderRecord& record, const Range& range)
{
	Record(this->ProcessRangeRequest(record, range));	
}

void APDUHandlerBase::OnCountRequest(const HeaderRecord& record, uint16_t count)
{
	Record(this->ProcessCountRequest(record, count));	
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var1Def& value)
{
	Record(this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var2Def& value)
{
	Record(this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var5Def& value)
{
	Record(this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var6Def& value)
{
	Record(this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<IINValue, uint16_t>>& values)
{
	Record(this->ProcessIIN(record, values));	
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const IterableBuffer<Group50Var1>& values)
{
	Record(ProcessCountOf(record, values));	
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var1>& values)
{
	Record(ProcessCountOf(record, values));
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var2>& values)
{
	Record(ProcessCountOf(record, values));
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const IterableBuffer<Group52Var2>& values)
{
	Record(ProcessCountOf(record, values));
}

void APDUHandlerBase::OnCountOf(const HeaderRecord& record, const IterableBuffer<Group120Var4>& values)
{
	Record(ProcessCountOf(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& values)
{
	Record(ProcessRange(record, values));	
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& values)
{
	Record(ProcessRange(record, values));	
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& values)
{
	Record(ProcessRange(record, values));	
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& values)
{
	Record(ProcessRange(record, values));	
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& values)
{
	Record(ProcessRange(record, values));	
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& values)
{
	Record(ProcessRange(record, values));	
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& values)
{
	Record(ProcessRange(record,  values));	
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& values)
{		
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& values)
{
	Record(ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& values)
{
	Record(ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& values)
{
	Record(ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& values)
{
	Record(ProcessRange(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& values)
{
	Record(ProcessIndexPrefix(record, values));
}

IINField APDUHandlerBase::ProcessAllObjects(const HeaderRecord& record)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRangeRequest(const HeaderRecord& record, const Range& range)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountRequest(const HeaderRecord& record, uint16_t count)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIIN(const HeaderRecord& record, const IterableBuffer<IndexedValue<IINValue, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var1Def& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var2Def& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var5Def& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var6Def& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group50Var1>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var1>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const opendnp3::IterableBuffer<Group51Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group52Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCountOf(const HeaderRecord& record, const IterableBuffer<Group120Var4>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<TimeAndInterval, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<BinaryCommandEvent, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogCommandEvent, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& values)
{
	return ProcessUnsupportedHeader();
}

}
