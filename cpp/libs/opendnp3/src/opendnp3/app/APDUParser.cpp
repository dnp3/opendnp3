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

#include "APDUParser.h"

#include "opendnp3/gen/QualifierCode.h"
#include "opendnp3/LogLevels.h"
#include "opendnp3/app/GroupVariationRecord.h"
#include "opendnp3/app/MeasurementFactory.h"

using namespace openpal;

namespace opendnp3
{

APDUParser::Result APDUParser::ParseTwoPass(const openpal::ReadOnlyBuffer& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger, Context context)
{
	if(pHandler)
	{
		// do a single pass without the callbacks to validate that the message is well formed
		auto result = ParseSinglePass(buffer, pLogger, nullptr, context);
		if (result == Result::OK)
		{
			return ParseSinglePass(buffer, nullptr, pHandler, context);
		}
		else
		{
			return result;
		}		
	}
	else
	{
		return ParseSinglePass(buffer, pLogger, pHandler, context);
	}
}

APDUParser::Result APDUParser::ParseSinglePass(const openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, IAPDUHandler* pHandler, Context context)
{
	ReadOnlyBuffer copy(buffer);
	while(copy.Size() > 0)
	{
		auto result = ParseHeader(copy, pLogger, context, pHandler);
		if (result != Result::OK)
		{
			return result;
		}
	}
	return Result::OK;
}

APDUParser::Result APDUParser::ParseHeader(ReadOnlyBuffer& buffer, openpal::Logger* pLogger, Context& context, IAPDUHandler* pHandler)
{
	if (buffer.Size() < 3)
	{
		SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_HEADER, "Not enough data for header");
		return Result::NOT_ENOUGH_DATA_FOR_HEADER;
	}
	else
	{
		uint8_t group = UInt8::ReadBuffer(buffer);
		uint8_t variation = UInt8::ReadBuffer(buffer);
		auto gv = GroupVariationRecord::GetRecord(group, variation);
		if (gv.enumeration == GroupVariation::UNKNOWN)
		{
			FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_UNKNOWN_GROUP_VAR, "Unknown object %i / %i", gv.group, gv.variation);
			return Result::UNKNOWN_OBJECT;
		}
		else
		{
			uint8_t rawQualifier = UInt8::ReadBuffer(buffer);
			QualifierCode qualifier = QualifierCodeFromType(rawQualifier);
			HeaderRecord record(gv, qualifier);			

			switch (qualifier)
			{

			case(QualifierCode::ALL_OBJECTS) :
				{
					if (pHandler)
					{
						pHandler->AllObjects(record);
					}		

					FORMAT_LOGGER_BLOCK(pLogger, context.Filters(),
						"%03u,%03u - %s - %s",
						group,
						variation,
						GroupVariationToString(gv.enumeration),
						QualifierCodeToString(qualifier));

					return Result::OK;
				}

			case(QualifierCode::UINT8_CNT) :
				return ParseCountHeader<UInt8>(buffer, pLogger, context, record, pHandler);

			case(QualifierCode::UINT16_CNT) :
				return ParseCountHeader<UInt16>(buffer, pLogger, context, record, pHandler);

			case(QualifierCode::UINT8_START_STOP) :
				return ParseRangeHeader<UInt8, uint16_t>(buffer, pLogger, context, record, pHandler);

			case(QualifierCode::UINT16_START_STOP) :
				return ParseRangeHeader<UInt16, uint32_t>(buffer, pLogger, context, record, pHandler);

			case(QualifierCode::UINT8_CNT_UINT8_INDEX) :
				return ParseIndexPrefixHeader<UInt8>(buffer, pLogger, context, record, pHandler);

			case(QualifierCode::UINT16_CNT_UINT16_INDEX) :
				return ParseIndexPrefixHeader<UInt16>(buffer, pLogger, context, record, pHandler);

			default:
				FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_UNKNOWN_QUALIFIER, "Unknown qualifier %x", rawQualifier);
				return Result::UNKNOWN_QUALIFIER;
			}
		}
	}
}

IndexedValue<Binary, uint16_t> APDUParser::BoolToBinary(const IndexedValue<bool, uint16_t>& v)
{
	return IndexedValue<Binary, uint16_t>(Binary(v.value), v.index);
}

IndexedValue<BinaryOutputStatus, uint16_t> APDUParser::BoolToBinaryOutputStatus(const IndexedValue<bool, uint16_t>& v)
{
	return IndexedValue<BinaryOutputStatus, uint16_t>(BinaryOutputStatus(v.value), v.index);
}

#define MACRO_PARSE_OBJECTS_WITH_RANGE(descriptor) \
	case(GroupVariation::descriptor): \
	return ParseRangeFixedSize(record, descriptor::Inst(), buffer, pLogger, range, pHandler);

APDUParser::Result APDUParser::ParseRangeOfObjects(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, const Range& range, IAPDUHandler* pHandler)
{
	switch(record.enumeration)
	{
	case(GroupVariation::Group1Var1):
		return ParseRangeAsBitField(buffer, pLogger, record, range, [pHandler, record](const IterableBuffer<IndexedValue<bool, uint16_t>>& values)
		{
			if(pHandler)
			{
				auto mapped = MapIterableBuffer<IndexedValue<bool, uint16_t>, IndexedValue<Binary, uint16_t>>(&values,
				              [](const IndexedValue<bool, uint16_t>& v)
				{
					return IndexedValue<Binary, uint16_t>(Binary(v.value), v.index);
				}
				                                                                                             );
				pHandler->OnRange(record, mapped);
			}
		});

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group1Var2);

	case(GroupVariation::Group3Var1) :
		return ParseRangeAsDoubleBitField(buffer, pLogger, record, range, [pHandler, record](const IterableBuffer<IndexedValue<DoubleBit, uint16_t>>& values)
		{
			if (pHandler)
			{
				auto mapped = MapIterableBuffer<IndexedValue<DoubleBit, uint16_t>, IndexedValue<DoubleBitBinary, uint16_t>>(&values,
				              [](const IndexedValue<DoubleBit, uint16_t>& v)
				{
					return IndexedValue<DoubleBitBinary, uint16_t>(DoubleBitBinary(v.value), v.index);
				}
				                                                                                                           );
				pHandler->OnRange(record, mapped);
			}
		});

	case(GroupVariation::Group10Var1):
		return ParseRangeAsBitField(buffer, pLogger, record, range, [pHandler, record](IterableBuffer<IndexedValue<bool, uint16_t>>& values)
		{
			if(pHandler)
			{
				auto mapped = MapIterableBuffer<IndexedValue<bool, uint16_t>, IndexedValue<BinaryOutputStatus, uint16_t>>(&values,
				              [](const IndexedValue<bool, uint16_t>& v)
				{
					return IndexedValue<BinaryOutputStatus, uint16_t>(BinaryOutputStatus(v.value), v.index);
				}
				                                                                                                         );
				pHandler->OnRange(record, mapped);
			}
		});

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group3Var2);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group10Var2);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group20Var6);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var6);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var9);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group21Var10);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var3);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var4);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var5);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group30Var6);

		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var1);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var2);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var3);
		MACRO_PARSE_OBJECTS_WITH_RANGE(Group40Var4);	

	case(GroupVariation::Group80Var1):
		return ParseRangeAsBitField(buffer, pLogger, record, range, [pHandler, record](const IterableBuffer<IndexedValue<bool, uint16_t>>& values)
		{
			if(pHandler)
			{
				pHandler->OnIIN(record, values);
			}
		});

	case(GroupVariation::Group110AnyVar):
		return ParseRangeOfOctetData(buffer, pLogger, record, range, pHandler);

	default:
		FORMAT_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_ILLEGAL_QUALIFIER_AND_OBJECT,
			"Unsupported qualifier/object - %s - %i / %i",
			QualifierCodeToString(record.qualifier), record.group, record.variation);

		return Result::INVALID_OBJECT_QUALIFIER;
	}
}

APDUParser::Result APDUParser::ParseCountOfObjects(openpal::ReadOnlyBuffer& buffer, openpal::Logger* pLogger, const HeaderRecord& record, uint16_t count, IAPDUHandler* pHandler)
{
	switch(record.enumeration)
	{
		case(GroupVariation::Group50Var1) :
			return ParseCountOf<Group50Var1>(buffer, pLogger, record, count, pHandler);

		case(GroupVariation::Group51Var1) :
			return ParseCountOf<Group51Var1>(buffer, pLogger, record, count, pHandler);

		case(GroupVariation::Group51Var2) :
			return ParseCountOf<Group51Var2>(buffer, pLogger, record, count, pHandler);

		case(GroupVariation::Group52Var2) :
			return ParseCountOf<Group52Var2>(buffer, pLogger, record, count, pHandler);
		default:
			return Result::INVALID_OBJECT_QUALIFIER;
	}		
}

APDUParser::Result APDUParser::ParseRangeOfOctetData(
    openpal::ReadOnlyBuffer& buffer,
    openpal::Logger* pLogger,
    const HeaderRecord& record,
    const Range& range,
    IAPDUHandler* pHandler)
{
	if (record.variation > 0)
	{
		uint32_t size = record.variation * range.Count();
		if (buffer.Size() < size)
		{
			SIMPLE_LOGGER_BLOCK_WITH_CODE(pLogger, flags::WARN, ALERR_INSUFFICIENT_DATA_FOR_OBJECTS, "Not enough data for specified octet objects");
			return Result::NOT_ENOUGH_DATA_FOR_OBJECTS;
		}
		else
		{
			if (pHandler)
			{
				auto collection = CreateLazyIterable<IndexedValue<OctetString, uint16_t>>(buffer, range.Count(), [record, range](ReadOnlyBuffer & buff, uint32_t pos)
				{
					OctetString octets(buff.Take(record.variation));
					IndexedValue<OctetString, uint16_t> value(octets, range.start + pos);
					buff.Advance(record.variation);
					return value;
				});
				pHandler->OnRange(record, collection);
			}
			buffer.Advance(size);
			return Result::OK;
		}
	}
	else
	{
		SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Octet string variation 0 may only be used in requests");
		return Result::INVALID_OBJECT;
	}

}

}

