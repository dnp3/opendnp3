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

#include "EventCount.h"

namespace opendnp3
{

EventCount::EventCount() : numClass1(0), numClass2(0), numClass3(0)
{}

EventCount::EventCount(uint32_t numClass1_, uint32_t numClass2_, uint32_t numClass3_) : 
	numClass1(numClass1_),
	numClass2(numClass2_),
	numClass3(numClass3_)
{}

void EventCount::Clear()
{
	numClass1 = numClass2 = numClass3 = 0;	
}

EventCount EventCount::Subtract(const EventCount& rhs) const
{
	return EventCount(numClass1 - rhs.numClass1, numClass2 - rhs.numClass2, numClass3 - rhs.numClass3);
}

bool EventCount::IsEmpty() const
{
	return (numClass1 | numClass2 | numClass3) != 0;
}

void EventCount::Increment(EventClass clazz)
{
	switch(clazz)
	{
		case(EventClass::EC1) :
			++numClass1;
			break;
		case(EventClass::EC2):
			++numClass2;
			break;
		case(EventClass::EC3):
			++numClass3;
			break;
	}
}

void EventCount::Decrement(EventClass clazz)
{
	switch (clazz)
	{
		case(EventClass::EC1) :
			--numClass1;
			break;
		case(EventClass::EC2) :
			--numClass2;
			break;
		case(EventClass::EC3) :
			--numClass3;
			break;
	}
}

}
