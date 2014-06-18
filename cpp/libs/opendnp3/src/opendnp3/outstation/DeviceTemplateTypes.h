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
#ifndef __DEVICE_TEMPLATE_TYPES_H_
#define __DEVICE_TEMPLATE_TYPES_H_

#include "opendnp3/app/PointClass.h"

namespace opendnp3
{

/// Configuration type that has a PointClass member
struct EventPointRecord
{
	EventPointRecord(PointClass aPointClass) : EventClass(aPointClass)
	{}

	EventPointRecord() : EventClass(CLASS_1) {}

	/// when the point changes, it will generate an event unless EventClass == CLASS_0
	PointClass EventClass;
};

/// DeadbandPointRecords have a parameter for controlling eventing tolerances
template <class T>
struct DeadbandPointRecord : public EventPointRecord
{

	DeadbandPointRecord(PointClass aPointClass, T aDeadband) :
		EventPointRecord(aPointClass),
		Deadband(aDeadband)
	{}

	DeadbandPointRecord() : Deadband(0) {}

	/// Points can change value within the deadband and not trigger events
	T Deadband;
};

}

#endif
