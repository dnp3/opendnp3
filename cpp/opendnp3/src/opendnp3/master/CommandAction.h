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
#ifndef __COMMAND_ACTION_H_
#define __COMMAND_ACTION_H_

#include "ICommandProcessor.h"

namespace opendnp3
{

class CommandErasure
{

public:
	static const uint32_t MAX_SIZE = 3 * sizeof(ControlRelayOutputBlock);

	CommandErasure();

	void Apply(ICommandProcessor* pProcessor);

protected:

	typedef void(*Invoke)(ICommandProcessor*, uint8_t*);

	CommandErasure(uint32_t size_, Invoke pInvoke_);

	uint32_t size;
	Invoke pInvoke;	
	uint8_t bytes[MAX_SIZE];
};

template <class Lambda>
class CommandAction : public CommandErasure
{
	static_assert(sizeof(Lambda) <= MAX_SIZE, "Lambda too big for command erasure");

	public:

	CommandAction(const Lambda& lambda) : CommandErasure(sizeof(Lambda), &ApplyLambda)
	{
		*reinterpret_cast<Lambda*>(bytes) = lambda;
	}

	static void ApplyLambda(ICommandProcessor* pProcessor, uint8_t* pBytes)
	{
		auto pLambda = reinterpret_cast<Lambda*>(pBytes);
		(*pLambda)(pProcessor);
	}	
};

template <class Lambda>
CommandErasure CreateAction(const Lambda& lambda)
{
	return CommandAction<Lambda>(lambda);
}

}

#endif

