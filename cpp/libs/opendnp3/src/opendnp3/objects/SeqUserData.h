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

#ifndef OPENDNP3_SEQUSERDATA_H
#define OPENDNP3_SEQUSERDATA_H

#include <openpal/logging/Logger.h>
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>

#include "opendnp3/app/GroupVariationID.h"

namespace opendnp3 {

/// Reusable structure that appears in multiple g120 variations
struct SeqUserData
{
	SeqUserData();

	SeqUserData(
		uint32_t seq,
		uint16_t user,		
		const openpal::ReadBufferView& data
	);	

	uint32_t Size() const;	
	
	uint32_t seq;
	uint16_t user;
	openpal::ReadBufferView data;

	protected:

	static bool Read(const openpal::ReadBufferView& buffer, SeqUserData& output);
	static bool Write(const SeqUserData& output, openpal::WriteBufferView& buffer);

	static const uint32_t MIN_SIZE = 6;	
};

}

#endif
