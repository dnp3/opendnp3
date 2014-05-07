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
#ifndef __POINT_CLASS_H_
#define __POINT_CLASS_H_

#include <cstdint>

namespace opendnp3
{

/// Bitmasks for Class1/2/3 events
enum PointClass : uint8_t
{
	CLASS_0 = 0x01,
	CLASS_1 = 0x02,
	CLASS_2 = 0x04,
	CLASS_3 = 0x08,
	ALL_EVENT_CLASSES = CLASS_1 | CLASS_2 | CLASS_3,
	ALL_CLASSES = CLASS_0 | ALL_EVENT_CLASSES
	
};

///converts an integer to a PointClass enumeration
PointClass IntToPointClass(uint8_t clazz);

}

#endif

