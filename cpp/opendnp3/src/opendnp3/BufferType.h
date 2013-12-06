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
#ifndef __BUFFER_TYPES_H_
#define __BUFFER_TYPES_H_

#include <opendnp3/MeasurementType.h>
#include <openpal/Visibility.h>

namespace opendnp3
{

/**
 * An enumeration that lists the various types of buffers that a
 * SlaveEventBuffer implements.  Note that each has a corresponding
 * data type in the enumeration DataTypes, with the exception of
 * VTO.
 */
enum class BufferType {
	BINARY,
	ANALOG,
	COUNTER,
	VTO,
	INVALID
};

/**
 * Maps a DataTypes enumeration value to a BufferTypes enumeration
 * value.
 *
 * @param aType			the DataTypes enumeration value
 *
 * @return				the corresponding BufferTypes enumeration
 * 						value
 * 
 */
BufferType DLL_LOCAL Convert(MeasurementType aType);

}

/* vim: set ts=4 sw=4: */

#endif
