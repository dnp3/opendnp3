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

#include "EventType.h"
#include "EventCount.h"

#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

class SelectionCriteria : private openpal::Uncopyable
{

public:
	
	SelectionCriteria();

	SelectionCriteria(const ClassField& field);

	SelectionCriteria(uint16_t clazz1, uint16_t clazz2, uint16_t clazz3);

	bool IsMatch(EventClass clazz, EventType type) const;

	void Clear();

	bool HasSelection() const;

	inline void AddToClass1(uint16_t types)
	{
		class1 |= types;
	}

	inline void AddToClass2(uint16_t types)
	{
		class2 |= types;
	}

	inline void AddToClass3(uint16_t types)
	{
		class3 |= types;
	}

	inline void AddToAllClasses(uint16_t types)
	{
		AddToClass1(types);
		AddToClass2(types);
		AddToClass3(types);
	}

private:

	// ----- bit masks for measurement types in each class --------

	uint16_t class1;
	uint16_t class2;
	uint16_t class3;
};

}

#endif
