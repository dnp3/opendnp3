//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
// contributor license agreements. See the NOTICE file distributed with this
// work for additional information regarding copyright ownership.  Green Enery
// Corp licenses this file to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
//

#ifndef __BUFFER_TYPES_H_
#define __BUFFER_TYPES_H_

#include <APL/DataTypes.h>

namespace apl
{
namespace dnp
{

/**
 * An enumeration that lists the various types of buffers that a
 * SlaveEventBuffer implements.  Note that each has a corresponding
 * data type in the enumeration DataTypes, with the exception of
 * BT_VTO.
 */
enum BufferTypes {
	BT_BINARY,
	BT_ANALOG,
	BT_COUNTER,
	BT_VTO,
	BT_INVALID		/* used for unit testing only, keep as last */
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
 * @throw ArgumentException		if the aType value is invalid
 */
BufferTypes Convert(DataTypes aType);

}
}

/* vim: set ts=4 sw=4: */

#endif
