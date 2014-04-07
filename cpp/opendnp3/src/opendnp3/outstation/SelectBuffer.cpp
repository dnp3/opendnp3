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

#include "SelectBuffer.h"

using namespace openpal;

namespace opendnp3
{

SelectBuffer::SelectBuffer(openpal::IExecutor* pExecutor_, const TimeDuration& selectTimeout_) :
	state(State::NoSelect),
	pExecutor(pExecutor_),
	selectTimeout(selectTimeout_),
	selectedSequence(0xFF),
	timestamp(MonotonicTimestamp::Min())
{

}

void SelectBuffer::Clear()
{
	state = State::NoSelect;
}

SelectBuffer::SelectResult SelectBuffer::Select(uint8_t sequence, const openpal::ReadOnlyBuffer& headers)
{
	switch (state)
	{
	case(State::Selected):
		{
			if (sequence == selectedSequence)
			{
				if (selectedBuffer.Equals(headers)) // this is a repeat
				{
					// don't restart timer, don't change states
					return SelectResult::REPEAT;
				}
				else
				{
					state = State::NoSelect;
					return SelectResult::PARAM_ERROR;
				}
			}
			else // this is a new select, so just record over top of the old one
			{
				return RecordSelect(sequence, headers);
			}
		}

	default:
		return RecordSelect(sequence, headers);
	}
}

SelectBuffer::SelectResult SelectBuffer::RecordSelect(uint8_t sequence, const openpal::ReadOnlyBuffer& headers)
{
	if (headers.Size() <= buffer.Size())
	{
		timestamp = pExecutor->GetTime();
		state = State::Selected;
		selectedSequence = sequence;
		headers.CopyTo(buffer.GetWriteBuffer());
		selectedBuffer = buffer.ToReadOnly().Truncate(headers.Size());
		return SelectResult::OK;
	}
	else
	{
		state = State::NoSelect;
		return SelectResult::PARAM_ERROR;
	}
}

SelectBuffer::OperateResult SelectBuffer::Operate(uint8_t sequence, const openpal::ReadOnlyBuffer& headers)
{
	auto elapsed = pExecutor->GetTime().milliseconds - timestamp.milliseconds;
	auto equal = selectedBuffer.Equals(headers);
	auto correctSequence = (sequence == ((selectedSequence + 1) % 16));	
	auto result = OperateResult::NO_SELECT;

	if(state == State::Selected)
	{
		if (equal && correctSequence)
		{
			if (elapsed > selectTimeout.GetMilliseconds())
			{
				state = State::NoSelect;
				result = OperateResult::TIMEOUT;
			}
			else
			{
				state = State::Operated;
				result = OperateResult::OK;
			}
		}
		else
		{
			state = State::NoSelect;
			result = OperateResult::NO_SELECT;
		}
	}
	else if(state == State::Operated)
	{
		if(equal && correctSequence)
		{
			result = OperateResult::REPEAT;
		}
		else
		{
			result = OperateResult::NO_SELECT;
		}
	}
	
	return result;
}

}

