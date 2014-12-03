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

#ifndef OPENDNP3_STATICLOADFUNCTIONS_H
#define OPENDNP3_STATICLOADFUNCTIONS_H

#include "opendnp3/app/Range.h"
#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/app/TimeAndInterval.h"
#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/outstation/MeasurementCell.h"

#include <openpal/container/ArrayView.h>

namespace opendnp3
{

template <class T>
struct StaticWriter
{
	typedef bool (*Function)(openpal::ArrayView<BufferedCell<T>, uint16_t>& view, HeaderWriter& writer, Range& range);
};

StaticWriter<Binary>::Function GetStaticWriter(StaticBinaryResponse variation);

StaticWriter<DoubleBitBinary>::Function GetStaticWriter(StaticDoubleBinaryResponse variation);

StaticWriter<Counter>::Function GetStaticWriter(StaticCounterResponse variation);

StaticWriter<FrozenCounter>::Function GetStaticWriter(StaticFrozenCounterResponse variation);

StaticWriter<Analog>::Function GetStaticWriter(StaticAnalogResponse variation);

StaticWriter<AnalogOutputStatus>::Function GetStaticWriter(StaticAnalogOutputStatusResponse variation);

StaticWriter<BinaryOutputStatus>::Function GetStaticWriter(StaticBinaryOutputStatusResponse variation);

StaticWriter<TimeAndInterval>::Function GetStaticWriter(StaticTimeAndIntervalResponse variation);

template <class Serializer>
bool WriteWithSerializer(openpal::ArrayView<BufferedCell<typename Serializer::Target>, uint16_t>& view, HeaderWriter& writer, Range& range)
{
	if (range.IsOneByte())
	{
		auto iter = writer.IterateOverRange<openpal::UInt8, typename Serializer::Target>(QualifierCode::UINT8_START_STOP, Serializer::Inst(), static_cast<uint8_t>(range.start));
		return LoadWithRangeIterator<typename Serializer::Target, openpal::UInt8>(view, iter, range);
	}
	else
	{
		auto iter = writer.IterateOverRange<openpal::UInt16, typename Serializer::Target>(QualifierCode::UINT16_START_STOP, Serializer::Inst(), range.start);
		return LoadWithRangeIterator<typename Serializer::Target, openpal::UInt16>(view, iter, range);
	}
}

template <class Target, class IndexType>
bool LoadWithRangeIterator(openpal::ArrayView<BufferedCell<Target>, uint16_t>& view, RangeWriteIterator<IndexType, Target>& iterator, Range& range)
{		
	// record the initial variation for the header, if it changes we can abort this header
	auto variation = view[range.start].variation;

	// TODO validate that the variation matches what's required
	while (range.IsValid() && view[range.start].selected && view[range.start].variation == variation)
	{			
		if (iterator.Write(view[range.start].value))
		{
			// deselect the value and advance the range
			view[range.start].selected = false;
			range.Advance();
		}
		else
		{
			return false;
		}		
	}

	return true;	
}

}

#endif

