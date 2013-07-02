
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
// you under the terms of the License.
//

#include "EnhancedVto.h"

#include "VtoData.h"

#include <opendnp3/Exception.h>
#include <opendnp3/Location.h>

#include <memory.h>
#include <stddef.h>

namespace opendnp3
{

const char EnhancedVto::MAGIC_BYTES[MAGIC_BYTES_SIZE] = {'\xAB', '\xBC', '\xCD'};

VtoData EnhancedVto::CreateVtoData(bool aLocalVtoConnectionOpened, uint8_t aChannelId)
{
	VtoData vto(2 + MAGIC_BYTES_SIZE);
	vto.mpData[0] = aChannelId;
	vto.mpData[1] = aLocalVtoConnectionOpened ? 0 : 1;
	memcpy(vto.mpData + 2, EnhancedVto::MAGIC_BYTES, MAGIC_BYTES_SIZE);
	return vto;
}

void EnhancedVto::ReadVtoData(const VtoData& arData, bool& arLocalVtoConnectionOpened, uint8_t& arChannelId)
{
	if(arData.GetSize() != (2 + MAGIC_BYTES_SIZE))
		MACRO_THROW_EXCEPTION(Exception, "Unexpected size in enhanced vto frame");

	if(memcmp(arData.mpData + 2, MAGIC_BYTES, MAGIC_BYTES_SIZE) != 0)
		MACRO_THROW_EXCEPTION(Exception, "Enhanced vto frame did not include the control sequence");

	arChannelId = arData.mpData[0];
	arLocalVtoConnectionOpened = (arData.mpData[1] == 0);
}

}

/* vim: set ts=4 sw=4: */

