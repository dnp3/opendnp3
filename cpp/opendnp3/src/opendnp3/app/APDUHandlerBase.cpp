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

APDUHandlerBase::APDUHandlerBase(openpal::Logger logger) :
	Loggable(logger),
	ignoredHeaders(0),
	errors(),
	cto(0),
	ctoHeader(-1),
	currentHeader(0)
{

}

void APDUHandlerBase::Reset()
{
	ignoredHeaders = 0;
	errors.Clear();
	cto = 0;
	ctoHeader = -1;
	currentHeader = 0;
}

IINField APDUHandlerBase::Errors() const
{
	IINField copy(errors);
	if(ignoredHeaders > 0) copy.Set(IINBit::FUNC_NOT_SUPPORTED);
	return copy;
}

void APDUHandlerBase::AllObjects(const GroupVariationRecord& record)
{
	this->_AllObjects(record);
	++currentHeader;
}

void APDUHandlerBase::OnRangeRequest(const GroupVariationRecord& record, const StaticRange& range)
{
	this->_OnRangeRequest(record, range);
	++currentHeader;
}

void APDUHandlerBase::OnCountRequest(const GroupVariationRecord& record, uint32_t count)
{
	this->_OnCountRequest(record, count);
	++currentHeader;
}

void APDUHandlerBase::OnIIN(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<bool, uint16_t>>& meas)
{
	this->_OnIIN(meas);
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const IterableBuffer<Group50Var1>& objects)
{
	this->_OnCountOf(objects);
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const opendnp3::IterableBuffer<Group51Var1>& times)
{
	Group51Var1 object;
	if (times.ReadOnlyValue(object))
	{
		cto = object.time;
		ctoHeader = currentHeader;
	}
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const opendnp3::IterableBuffer<Group51Var2>& times)
{
	Group51Var2 object;
	if (times.ReadOnlyValue(object))
	{
		cto = object.time;
		ctoHeader = currentHeader;
	}
	++currentHeader;
}

void APDUHandlerBase::OnCountOf(const IterableBuffer<Group52Var2>& objects)
{
	this->_OnCountOf(objects);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	if (record.enumeration == GroupVariation::Group2Var3)
	{
		this->OnIndexPrefixCTO(record, qualifier, meas);
	}
	else
	{
		this->_OnIndexPrefix(record, meas);
	}

	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	if (record.enumeration == GroupVariation::Group4Var3)
	{
		this->OnIndexPrefixCTO(record, qualifier, meas);
	}
	else
	{
		this->_OnIndexPrefix(record, meas);
	}

	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	this->_OnIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	this->_OnIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	this->_OnIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas)
{
	this->_OnIndexPrefix(meas);
	++currentHeader;
}

void APDUHandlerBase::OnRange(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	this->_OnRange(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	this->_OnIndexPrefix(record, meas);
	++currentHeader;
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint8_t>>& meas)
{
	auto transform = MapIterableBuffer<IndexedValue<Binary, uint8_t>, IndexedValue<Binary, uint16_t>>(meas,
	                 [](const IndexedValue<Binary, uint8_t>& value)
	{
		return value.Widen<uint16_t>();
	}
	                                                                                                 );
	this->OnIndexPrefix(record, qualifier, transform);
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint8_t>>& meas)
{
	auto transform = MapIterableBuffer<IndexedValue<DoubleBitBinary, uint8_t>, IndexedValue<DoubleBitBinary, uint16_t>>(meas,
	                 [](const IndexedValue<DoubleBitBinary, uint8_t>& value)
	{
		return value.Widen<uint16_t>();
	}
	                                                                                                                   );
	this->OnIndexPrefix(record, qualifier, transform);
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint8_t>>& meas)
{
	auto transform = MapIterableBuffer<IndexedValue<Counter, uint8_t>, IndexedValue<Counter, uint16_t>>(meas,
	                 [](const IndexedValue<Counter, uint8_t>& value)
	{
		return value.Widen<uint16_t>();
	}
	                                                                                                   );
	this->OnIndexPrefix(record, qualifier, transform);
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint8_t>>& meas)
{
	auto transform = MapIterableBuffer<IndexedValue<FrozenCounter, uint8_t>, IndexedValue<FrozenCounter, uint16_t>>(meas,
	                 [](const IndexedValue<FrozenCounter, uint8_t>& value)
	{
		return value.Widen<uint16_t>();
	}
	                                                                                                               );
	this->OnIndexPrefix(record, qualifier, transform);
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint8_t>>& meas)
{
	auto transform = MapIterableBuffer<IndexedValue<Analog, uint8_t>, IndexedValue<Analog, uint16_t>>(meas,
	                 [](const IndexedValue<Analog, uint8_t>& value)
	{
		return value.Widen<uint16_t>();
	}
	                                                                                                 );
	this->OnIndexPrefix(record, qualifier, transform);
}

void APDUHandlerBase::OnIndexPrefix(const GroupVariationRecord& record, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint8_t>>& meas)
{
	auto transform = MapIterableBuffer<IndexedValue<OctetString, uint8_t>, IndexedValue<OctetString, uint16_t>>(meas,
	                 [](const IndexedValue<OctetString, uint8_t>& value)
	{
		return value.Widen<uint16_t>();
	}
	                                                                                                           );
	this->OnIndexPrefix(record, qualifier, transform);
}
void APDUHandlerBase::_AllObjects(const GroupVariationRecord& record)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRangeRequest(const GroupVariationRecord& record, const StaticRange& range)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnCountRequest(const GroupVariationRecord& record, uint32_t count)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIIN(const IterableBuffer<IndexedValue<bool, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnCountOf(const IterableBuffer<Group50Var1>& objects)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnCountOf(const IterableBuffer<Group52Var2>&)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	++ignoredHeaders;
}


void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnRange(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const GroupVariationRecord& record, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas)
{
	++ignoredHeaders;
}


void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas)
{
	++ignoredHeaders;
}

void APDUHandlerBase::_OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas)
{
	++ignoredHeaders;
}



bool APDUHandlerBase::GetCTO(uint64_t& cto_)
{
	if(ctoHeader >= 0 && (currentHeader == (ctoHeader + 1)))
	{
		cto_ = cto;
		return true;
	}
	else return false;
}



}


