
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __DEVICE_TEMPLATE_TYPES_H_
#define __DEVICE_TEMPLATE_TYPES_H_

#include <vector>

#include "Types.h"
#include "PointClass.h"

namespace opendnp3
{

struct PointRecord {
	PointRecord() {}
};

/// Configuration type that has a PointClass member
struct EventPointRecord : public PointRecord {
	EventPointRecord(PointClass aPointClass) :
		PointRecord(),
		EventClass(aPointClass)
	{}

	EventPointRecord() : EventClass(PC_CLASS_1) {}

	/// when the point changes, it will generate an event unless EventClass == PC_CLASS_0
	PointClass EventClass;
};

/// DeadbandPointRecords have a parameter for controlling eventing tolerances
struct DeadbandPointRecord : public EventPointRecord {

	DeadbandPointRecord(PointClass aPointClass, double aDeadband) :
		EventPointRecord(aPointClass),
		Deadband(aDeadband)
	{}

	DeadbandPointRecord() : Deadband(0) {}

	/// Points can change value within the deadband and not trigger events
	double Deadband;
};

}

#endif
