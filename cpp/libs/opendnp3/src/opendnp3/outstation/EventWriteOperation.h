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

#ifndef OPENDNP3_EVENTWRITEOPERATION_H
#define OPENDNP3_EVENTWRITEOPERATION_H

#include <cstdint>

#include <openpal/util/Uncopyable.h>
#include <openpal/container/StaticLinkedList.h>
#include <openpal/serialization/Serialization.h>
#include <openpal/executor/Function1.h>

#include "opendnp3/app/ClassField.h"
#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/outstation/SOERecord.h"
#include "opendnp3/outstation/EventResponseConfig.h"

namespace opendnp3
{

class HeaderWriter;
class SelectionCriteria;

class EventWriteLimits
{
	public:	
	
	EventWriteLimits() : numClass1(0), numClass2(0), numClass3(0), numOfType(0)
	{}

	void Decrement(EventClass clazz)
	{
		--numOfType;
		switch (clazz)
		{
			case(EventClass::EC1) :
				--numClass1;
				break;
			case(EventClass::EC2):
				--numClass2;
				break;
			case(EventClass::EC3):
				--numClass3;
				break;
		}
	}

	bool CanWrite(EventClass clazz) const
	{
		if (numOfType > 0)
		{
			switch (clazz)
			{
				case(EventClass::EC1) :
					return numClass1 > 0;
				case(EventClass::EC2) :
					return numClass2 > 0;
				case(EventClass::EC3) :
					return numClass3 > 0;
				default:
					return false;
			}
		}
		else
		{
			return false;
		}		
	}

	void Clear()
	{
		numClass1 = numClass2 = numClass3 = numOfType = 0;
	}

	uint32_t numClass1;
	uint32_t numClass2;
	uint32_t numClass3;

	uint32_t numOfType;
};

typedef openpal::Function1<openpal::ListNode<SOERecord>*, openpal::ListNode<SOERecord>*> ListIterator;

typedef bool(*EventHeaderWriteFunc)(const EventWriteLimits& classMask, HeaderWriter& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback);

class EventWriteOperation
{
	public:

	EventWriteOperation();

	EventWriteOperation(EventHeaderWriteFunc pWriter_, const EventWriteLimits& limits_);

	bool Invoke(HeaderWriter& writer, openpal::ListNode<SOERecord>* start, const ListIterator& writeCallback);

	bool IsDefined() const;

	void MarkAsWritten(uint32_t count);
	
	void Clear();	

	EventHeaderWriteFunc pWriter;	
	EventWriteLimits limits;
};

}

#endif
