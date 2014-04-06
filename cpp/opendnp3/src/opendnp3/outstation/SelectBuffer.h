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
#ifndef __SELECT_BUFFER_H_
#define __SELECT_BUFFER_H_

#include <openpal/StaticBuffer.h>
#include <openpal/IExecutor.h>

#include "opendnp3/StaticSizeConfiguration.h"

namespace opendnp3
{

class SelectBuffer
{

public:

	enum class SelectResult
	{
	    REPEAT,
	    PARAM_ERROR,
	    OK
	};

	enum class OperateResult
	{
	    REPEAT,
	    NO_SELECT,
	    TIMEOUT,
	    OK
	};

	SelectBuffer(openpal::IExecutor* pExecutor_, const openpal::TimeDuration& selectTimeout_);

	SelectResult Select(uint8_t sequence, const openpal::ReadOnlyBuffer& headers);

	OperateResult Operate(uint8_t sequence, const openpal::ReadOnlyBuffer& headers);

	void Clear();

private:

	enum class State
	{
	    NoSelect,
	    Selected,
	    Operated
	};

	SelectResult RecordSelect(uint8_t sequence, const openpal::ReadOnlyBuffer& headers);

	State state;
	openpal::IExecutor* pExecutor;
	const openpal::TimeDuration selectTimeout;
	uint8_t selectedSequence;
	openpal::MonotonicTimestamp timestamp;
	

	openpal::StaticBuffer<sizes::MAX_APDU_BUFFER_SIZE> buffer;
	openpal::ReadOnlyBuffer selectedBuffer;
};

}

#endif