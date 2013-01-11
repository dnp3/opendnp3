//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __DEVICE_TEMPLATE_TYPES_H_
#define __DEVICE_TEMPLATE_TYPES_H_

#include <vector>

#include <APL/Types.h>
#include <APL/CommandTypes.h>
#include "PointClass.h"

namespace apl
{
namespace dnp
{

// base class - all point types have a name
struct PointRecord {
	PointRecord(const std::string& arName) : Name(arName) {}
	PointRecord() {}

	std::string Name;
};

// Event point records also have a class
struct EventPointRecord : public PointRecord {
	EventPointRecord(const std::string& arName, PointClass aPointClass) :
		PointRecord(arName),
		EventClass(aPointClass)
	{}

	EventPointRecord() : EventClass(PC_CLASS_1) {}

	// when the point changes, it will generate an event unless EventClass == PC_CLASS_0
	PointClass EventClass;
};

// Adds a deadband parameter
struct DeadbandPointRecord : public EventPointRecord {
	DeadbandPointRecord(const std::string& arName, PointClass aPointClass, double aDeadband) :
		EventPointRecord(arName, aPointClass),
		Deadband(aDeadband)
	{}

	DeadbandPointRecord() : Deadband(0) {}

	// Points can change value within the deadband and not trigger events
	double Deadband;
};

// Todo - Add properties that determine how controls are handled - i.e. DO/SBO/etc
struct ControlRecord : public PointRecord {
	ControlRecord(const std::string& arName = "", CommandModes aMode = CM_SBO_ONLY, millis_t aSelectTimeoutMS = 5000) :
		PointRecord(arName),
		CommandMode(aMode),
		SelectTimeoutMS(aSelectTimeoutMS)
	{}

	apl::CommandModes CommandMode;
	millis_t SelectTimeoutMS;
};


}
}

#endif
