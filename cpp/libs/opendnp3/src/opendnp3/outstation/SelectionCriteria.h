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

#ifndef __SELECTION_CRITERIA_H_
#define __SELECTION_CRITERIA_H_

#include "opendnp3/app/EventType.h"
#include "opendnp3/outstation/EventCount.h"
#include "opendnp3/app/ClassField.h"
#include "opendnp3/gen/GroupVariation.h"

#include "opendnp3/outstation/EventWriteOperation.h"
#include "opendnp3/outstation/EventResponseConfig.h"
#include "opendnp3/app/IINField.h"

#include <openpal/util/Limits.h>
#include <openpal/util/Uncopyable.h>

namespace opendnp3
{



class SelectionCriteria : private openpal::Uncopyable
{

public:
	
	SelectionCriteria();

	SelectionCriteria(const ClassField& field);

	IINField RecordAllObjects(GroupVariation enumeration);

	EventWriteOperation GetWriteOperationFor(const EventResponseConfig& config, EventClass clazz, EventType type);

	void RecordAsWritten(EventClass clazz, EventType type);

	void Clear();

	bool HasSelection() const;	

private:	

	IINField SelectionCriteria::RecordClass(uint32_t& count);

	IINField SelectionCriteria::RecordClass(uint32_t& count, uint32_t value);

	static EventHeaderWriteFunc GetDefaultWriteFunction(const EventResponseConfig& config, EventType type);

	// ----- bit masks for measurement types in each class --------

	uint32_t numClass1;
	uint32_t numClass2;
	uint32_t numClass3;
};

}

#endif
