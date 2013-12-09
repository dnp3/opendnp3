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
#ifndef __POINT_INFO_H_
#define __POINT_INFO_H_

#include "Event.h"

#include <opendnp3/IndexedValue.h>

#include "MeasurementHelpers.h"

namespace opendnp3
{

/**
 * Extends an event structure wtih a lastEvent value
 * event value to the base class.
 */
template<typename T>
class LastEvent : public Event<T> 
{
	public:

	LastEvent(const T& aValue, uint32_t aIndex, PointClass aClass) :
		Event<T>(aValue, aIndex, aClass),
		lastEvent(aValue)		
	{}

	LastEvent() : Event<T>()
	{}
		
	T lastEvent;		// the last value that was reported	
};

/**
 * Structure for holding static data information. Adds a deadband and a last
 * event value to the base class.
 */
template<typename T>
class PointInfo : public LastEvent<T> 
{
	public:

	PointInfo(const T& aValue, uint32_t aIndex, PointClass aClass) :
		LastEvent<T>(aValue, aIndex, aClass),		
		deadband(0)
	{}

	PointInfo() : deadband(0)
	{}

	bool IsEvent(const T& aNewValue) const
	{
		return this->HasEventClass() && IsChangeEvent(aNewValue, lastEvent, deadband);
	}
		
	double deadband; // deadband associated with measurement (optional)
};

template <>
class PointInfo<Binary> : public LastEvent<Binary>
{
	public:

	PointInfo(const Binary& aValue, uint32_t aIndex, PointClass aClass) :
		LastEvent<Binary>(aValue, aIndex, aClass)		
	{}

	PointInfo()
	{}

	bool IsEvent(const Binary& aValue) const
	{
		if(this->HasEventClass())
		{
			return aValue.GetQuality() != lastEvent.GetQuality();
		}
		else return false;		
	}
};



} //end namespace

/* vim: set ts=4 sw=4: */

#endif

