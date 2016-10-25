/*
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
#ifndef ASIODNP3_STACKLIFECYCLE_H
#define ASIODNP3_STACKLIFECYCLE_H

#include "IStackLifecycle.h"

#include <set>

namespace asiopal
{
class ASIOExecutor;
}

namespace asiodnp3
{

class LinkLayerRouter;

class StackLifecycle final : public IStackLifecycle
{
public:

	StackLifecycle(LinkLayerRouter& router, asiopal::ASIOExecutor& executor);

	/// --- helper methods uses within the channel ----

	void Add(IStack* pStack);

	void ShutdownAll();

	/// --- implement IStackLifecycle ----

	virtual asiopal::ASIOExecutor& GetExecutor() override
	{
		return *pExecutor;
	}

	virtual bool EnableRoute(opendnp3::ILinkSession* pContext) override;

	virtual bool DisableRoute(opendnp3::ILinkSession* pContext) override;

	virtual void Shutdown(opendnp3::ILinkSession* pContext, IStack* pStack) override;


private:

	LinkLayerRouter* pRouter;
	asiopal::ASIOExecutor* pExecutor;
	std::set<IStack*> stacks;

};

}

#endif
