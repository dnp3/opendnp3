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
 * Structure for holding static data information. Adds a deadband and a last
 * event value to the base class.
 */
template<typename T>
struct PointInfo : public Event<T> 
{	
	PointInfo() : deadband(0) {}

	bool Load(const T& aNewValue)
	{
		auto event = IsChangeEvent(aNewValue, lastEvent, deadband);
		if(event) lastEvent = aNewValue;
		this->value = aNewValue;
		return this->HasEventClass() && event;			
	}
		
	T lastEvent;	 // the last value that was reported
	double deadband; // deadband associated with measurement (optional)
};

template <>
struct PointInfo<ControlStatus> : public IndexedValue<ControlStatus>
{
		PointInfo(){}
};

template <>
struct PointInfo<SetpointStatus> : public IndexedValue<SetpointStatus>
{
		PointInfo(){}
};

template <>
struct PointInfo<Binary> : public Event<Binary>
{
	public:
	
	PointInfo() {}
	
	bool Load(const Binary& aValue)
	{		
		auto event = aValue.GetQuality() != value.GetQuality();
		value = aValue;
		return HasEventClass() && event;		
	}
};



} //end namespace

/* vim: set ts=4 sw=4: */

#endif

