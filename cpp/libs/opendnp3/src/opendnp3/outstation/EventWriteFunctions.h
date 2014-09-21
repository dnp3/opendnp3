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

#ifndef OPENDNP3_EVENTWRITEFUNCTIONS_H
#define OPENDNP3_EVENTWRITEFUNCTIONS_H

#include "EventWriteOperation.h"

namespace opendnp3
{

class EventWriteFunctions : openpal::PureStatic
{

public:

	template <class Serializer>
	static bool WriteFixedSize(const EventWriteLimits& limits, HeaderWriter& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback);

	template <class Serializer, class CTOType>
	static bool WriteFixedSizeWithCTO(const EventWriteLimits& limits, HeaderWriter& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback);

private:

	template <class Target>
	static bool WriteFixedSizeWithPrefixIterator(const EventWriteLimits& limits, PrefixedWriteIterator<openpal::UInt16, Target>& iterator, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback);

	template <class Target, class CTOType>
	static bool WriteFixedSizeWithPrefixIteratorAndCTO(const EventWriteLimits& limits, PrefixedWriteIterator<openpal::UInt16, Target>& iterator, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback, const CTOType& cto);

};

template <class Serializer>
bool EventWriteFunctions::WriteFixedSize(const EventWriteLimits& limits, HeaderWriter& ow, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback)
{		
	auto iter = ow.IterateOverCountWithPrefix<openpal::UInt16, typename Serializer::Target>(QualifierCode::UINT16_CNT_UINT16_INDEX, Serializer::Inst());
	
	return WriteFixedSizeWithPrefixIterator(limits, iter, start, writeCallback);
}

template <class Serializer, class CTOType>
bool EventWriteFunctions::WriteFixedSizeWithCTO(const EventWriteLimits& limits, HeaderWriter& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback)
{
	typename Serializer::Target meas;
	start->value.Read(meas);
	CTOType cto = { meas.time };
	//cto.time = meas.time;

	auto iter = writer.IterateOverCountWithPrefixAndCTO<openpal::UInt16, typename Serializer::Target>(QualifierCode::UINT16_CNT_UINT16_INDEX, Serializer::Inst(), cto);

	return WriteFixedSizeWithPrefixIteratorAndCTO(limits, iter, start, writeCallback, cto);
}

template <class Target>
bool EventWriteFunctions::WriteFixedSizeWithPrefixIterator(const EventWriteLimits& limits, PrefixedWriteIterator<openpal::UInt16, Target>& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback)
{
	/// initialize with the limits, and decrement as write items with certain classes
	EventWriteLimits remainder(limits);

	if (writer.IsValid())
	{
		auto pNode = start;

		Target measurement;

		while (pNode && (pNode->value.type == Target::EventTypeEnum) && remainder.CanWrite(pNode->value.clazz))
		{
			pNode->value.Read(measurement);

			if (writer.Write(measurement, pNode->value.index))
			{
				remainder.Decrement(pNode->value.clazz);
				// inform that we wrote this node, and get the next node
				pNode = writeCallback.Apply(pNode);
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		// can't even write the header + one measurement
		return false;
	}
}

template <class Target, class CTOType>
bool EventWriteFunctions::WriteFixedSizeWithPrefixIteratorAndCTO(const EventWriteLimits& limits, PrefixedWriteIterator<openpal::UInt16, Target>& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback, const CTOType& cto)
{
	/// initialize with the limits, and decrement as write items with certain classes
	EventWriteLimits remainder(limits);

	if (writer.IsValid())
	{
		auto pNode = start;

		Target measurement;		

		while (pNode && (pNode->value.type == Target::EventTypeEnum) && remainder.CanWrite(pNode->value.clazz))
		{
			pNode->value.Read(measurement);

			if (measurement.time < cto.time)
			{
				// can't write this because the timestamp is less than the CTO
				return true;
			}
			else
			{
				auto diff = measurement.time - cto.time;
				if (diff > openpal::UInt16::Max)
				{
					// can't write this measurement because the timestamp difference is buffer than 2^16 - 1
					return true;
				}
				else
				{					
					measurement.time = diff;
					if (writer.Write(measurement, pNode->value.index))
					{
						remainder.Decrement(pNode->value.clazz);
						// inform that we wrote this node, and get the next node
						pNode = writeCallback.Apply(pNode);
					}
					else
					{
						return false;
					}
				}
			}			
		}

		return true;
	}
	else
	{
		// can't even write the header + one measurement
		return false;
	}
}


}

#endif
