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
template <typename T, typename U, typename V>
struct DeadbandPointInfo : public Event<T> 
{	
	DeadbandPointInfo() : deadband(0) {}

	bool Load(const T& aValue)
	{
		auto qualityChange = value.GetQuality() != aValue.GetQuality();
		auto event = qualityChange || ExceedsDeadband<U,V>(aValue.GetValue(), lastEvent.GetValue(), deadband);
		if(event) lastEvent = aValue;
		this->value = aValue;
		return event;			
	}
		
	T lastEvent;	 // the last value that was reported
	U deadband; // deadband associated with measurement (optional)
};

template <typename T>
struct PointInfo : public DeadbandPointInfo<T, double, double>
{	
	PointInfo(){}
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
		return event;		
	}
};



} //end namespace

/* vim: set ts=4 sw=4: */

#endif

