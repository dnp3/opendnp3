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

#ifndef OPENDNP3_SELECTIONCRITERIA_H
#define OPENDNP3_SELECTIONCRITERIA_H

#include "opendnp3/app/EventType.h"
#include "opendnp3/outstation/EventCount.h"
#include "opendnp3/app/ClassField.h"
#include "opendnp3/gen/GroupVariation.h"

#include "opendnp3/outstation/EventWriteOperation.h"
#include "opendnp3/outstation/EventResponseConfig.h"
#include "opendnp3/outstation/EventResponseTypes.h"
#include "opendnp3/app/IINField.h"

#include <openpal/util/Limits.h>
#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

class SelectionCriteria : private openpal::Uncopyable
{
	// requests can be class based, or type based, but not both
	// as the appropriate response to this would not be clearly defined
	enum Mode
	{
		UNDEFINED,
		CLASS_BASED,
		TYPE_BASED,
		FAILED
	};

	class TypeSelection
	{
		public:

		TypeSelection();

		TypeSelection(uint32_t maximum_, EventHeaderWriteFunc function_);

		bool IsDefined() const;

		void Clear();

		uint32_t maximum;
		EventHeaderWriteFunc function;
	};

public:
	
	SelectionCriteria(const EventResponseConfig& config);

	IINField RecordAllObjects(GroupVariation enumeration);

	IINField RecordCountOfObjects(GroupVariation enumeration, uint32_t count);

	void RecordViaClassField(const ClassField& field);

	EventWriteOperation GetWriteOperationFor(EventClass clazz, EventType type);

	void RecordAsWritten(EventClass clazz, EventType type);

	void Clear();

	bool HasSelection() const;	

private:	

	EventWriteOperation GetClassBasedWriteOperationFor(EventClass clazz, EventType type);	

	EventWriteOperation GetTypedBasedWriteOperationFor(EventClass clazz, EventType type);

	void AssignAllClassCounts(uint32_t count);

	void ClearAllTypeSections();

	bool HasTypeSelection() const;

	IINField RecordClass(EventClass ec, uint32_t count);
	
	IINField RecordEventType(EventType type, EventHeaderWriteFunc function, uint32_t count);	

	static EventHeaderWriteFunc GetDefaultWriteFunction(const EventResponseConfig& config, EventType type);

	template <class Target>
	IINField RecordType(typename Target::EventResponseEnum enumeration, uint32_t count)
	{
		auto function = EventResponseTypes::Lookup(enumeration);
		return RecordEventType(Target::EventTypeEnum, function, count);
	}

	Mode mode;
	EventResponseConfig defaults;

	// ----- allowed counts for class mode ----------

	uint32_t classCounts[3];

	// ----- allowed selectionsfor type mode --------

	TypeSelection typeSelections[NUM_OUTSTATION_EVENT_TYPES];
};

}

#endif
