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
	Record(record, this->ProcessAllObjects(record));	
}

void APDUHandlerBase::OnRangeRequest(const HeaderRecord& record, const Range& range)
{
	Record(record, this->ProcessRangeRequest(record, range));
}

void APDUHandlerBase::OnCountRequest(const HeaderRecord& record, uint16_t count)
{
	Record(record, this->ProcessCountRequest(record, count));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var1& value)
{
	Record(record, this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var2& value)
{
	Record(record, this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var5& value)
{
	Record(record, this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnFreeFormat(const HeaderRecord& record, const Group120Var6& value)
{
	Record(record, this->ProcessFreeFormat(record, value));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const IINValue& value, uint16_t index)
{
	Record(record, this->ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnCount(const HeaderRecord& record, const ICollection<Group50Var1>& values)
{
	Record(record, this->ProcessCount(record, values));
}

void APDUHandlerBase::OnCount(const HeaderRecord& record, const ICollection<Group51Var1>& values)
{
	Record(record, this->ProcessCount(record, values));
}

void APDUHandlerBase::OnCount(const HeaderRecord& record, const ICollection<Group51Var2>& values)
{
	Record(record, this->ProcessCount(record, values));
}

void APDUHandlerBase::OnCount(const HeaderRecord& record, const ICollection<Group52Var2>& values)
{
	Record(record, this->ProcessCount(record, values));
}

void APDUHandlerBase::OnCount(const HeaderRecord& record, const ICollection<Group120Var4>& values)
{
	Record(record, this->ProcessCount(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const Binary& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const Counter& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const FrozenCounter& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const Analog& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const OctetString& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const TimeAndInterval& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, uint32_t count, const Group121Var1& value, uint16_t index)
{
	Record(record, ProcessRange(record, count, value, index));
}

// --- index prefixes ----

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Binary& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Counter& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const FrozenCounter& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Analog& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const OctetString& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const TimeAndInterval& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryCommandEvent& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogCommandEvent& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var1& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var2& value, uint16_t index)
{
	Record(record, ProcessIndexPrefix(record, count, value, index));
}

// --- controls ----

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{		
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
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

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var5& value)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value)
{
	return ProcessUnsupportedHeader();
}

/// ---- counts -----

IINField APDUHandlerBase::ProcessCount(const HeaderRecord& record, const ICollection<Group50Var1>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCount(const HeaderRecord& record, const ICollection<Group51Var1>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCount(const HeaderRecord& record, const ICollection<Group51Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCount(const HeaderRecord& record, const ICollection<Group52Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessCount(const HeaderRecord& record, const ICollection<Group120Var4>&)
{
	return ProcessUnsupportedHeader();
}

/// ---- ranges -----

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const IINValue& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const Binary& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const Counter& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const FrozenCounter& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const Analog& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const OctetString& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const TimeAndInterval& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, uint32_t count, const Group121Var1& value, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

/// ---- index prefixes -----

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Counter& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const FrozenCounter& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Binary& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Analog& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const OctetString& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const TimeAndInterval& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryCommandEvent& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogCommandEvent& values, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var1& meas, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var2& meas, uint16_t index)
{
	return ProcessUnsupportedHeader();
}

//// --- controls ----

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
	return ProcessUnsupportedHeader();
}

}
