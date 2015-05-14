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

void APDUHandlerBase::OnHeader(const AllObjectsHeader& header)
{
	Record(header, this->ProcessHeader(header));	
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

void APDUHandlerBase::OnValues(const CountHeader& header, const ICollection<Group50Var1>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const CountHeader& header, const ICollection<Group51Var1>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const CountHeader& header, const ICollection<Group51Var2>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const CountHeader& header, const ICollection<Group52Var2>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const CountHeader& header, const ICollection<Group120Var4>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<IINValue>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<TimeAndInterval>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

void APDUHandlerBase::OnRange(const HeaderRecord& record, const ICollection<Indexed<Group121Var1>>& values)
{
	Record(record, this->ProcessRange(record, values));
}

// --- index prefixes ----

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<TimeAndInterval>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Group122Var1>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
}

void APDUHandlerBase::OnIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Group122Var2>>& values)
{
	Record(record, ProcessIndexPrefix(record, values));
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

IINField APDUHandlerBase::ProcessHeader(const AllObjectsHeader& record)
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

IINField APDUHandlerBase::ProcessValues(const CountHeader& header, const ICollection<Group50Var1>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const CountHeader& header, const ICollection<Group51Var1>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const CountHeader& header, const ICollection<Group51Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const CountHeader& header, const ICollection<Group52Var2>&)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const CountHeader& header, const ICollection<Group120Var4>&)
{
	return ProcessUnsupportedHeader();
}

/// ---- ranges -----

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<IINValue>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessRange(const HeaderRecord& record, const ICollection<Indexed<Group121Var1>>& values)
{
	return ProcessUnsupportedHeader();
}

/// ---- index prefixes -----

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Counter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<FrozenCounter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Binary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Analog>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<OctetString>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Group122Var1>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessIndexPrefix(const HeaderRecord& record, const ICollection<Indexed<Group122Var2>>& values)
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
