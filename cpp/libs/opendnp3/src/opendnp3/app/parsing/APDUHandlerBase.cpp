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

APDUHandlerBase::APDUHandlerBase() :	
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

void APDUHandlerBase::OnHeader(const RangeHeader& header)
{
	Record(header, this->ProcessHeader(header));
}

void APDUHandlerBase::OnHeader(const CountHeader& header)
{
	Record(header, this->ProcessHeader(header));
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

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<Binary>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<Counter>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

// --- index prefixes ----

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

// --- controls ----

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{		
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

void APDUHandlerBase::OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
	Record(header, this->ProcessValues(header, values));
}

IINField APDUHandlerBase::ProcessHeader(const AllObjectsHeader& record)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessHeader(const RangeHeader& header)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessHeader(const CountHeader& header)
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

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<Binary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<Counter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values)
{
	return ProcessUnsupportedHeader();
}

/// ---- index prefixes -----

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values)
{
	return ProcessUnsupportedHeader();
}

//// --- controls ----

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values)
{
	return ProcessUnsupportedHeader();
}

IINField APDUHandlerBase::ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values)
{
	return ProcessUnsupportedHeader();
}

}
