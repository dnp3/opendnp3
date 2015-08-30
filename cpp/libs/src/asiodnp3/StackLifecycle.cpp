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

#include "StackLifecycle.h"

#include <openpal/executor/IExecutor.h>
#include <asiopal/ASIOExecutor.h>

#include "asiodnp3/LinkLayerRouter.h"

#include <vector>

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

StackLifecycle::StackLifecycle(LinkLayerRouter& router, asiopal::ASIOExecutor& executor) :
	pRouter(&router),
	pExecutor(&executor)
{

}

void StackLifecycle::Add(IStack* pStack)
{
	stacks.insert(pStack);
}

void StackLifecycle::ShutdownAll()
{
	// make a copy of all the stacks
	std::vector<IStack*> stackscopy;
	for (auto pStack : stacks) stackscopy.push_back(pStack);

	for (auto pStack : stackscopy)
	{
		pStack->Shutdown();
	}

	assert(stacks.empty());
}

bool StackLifecycle::EnableRoute(ILinkSession* pContext)
{
	auto enable = [this, pContext]()
	{
		return pRouter->Enable(pContext);
	};
	return pExecutor->ReturnBlockFor<bool>(enable);
}

bool StackLifecycle::DisableRoute(ILinkSession* pContext)
{
	auto disable = [this, pContext]()
	{
		return pRouter->Disable(pContext);
	};
	return pExecutor->ReturnBlockFor<bool>(disable);
}

void StackLifecycle::Shutdown(ILinkSession* pContext, IStack* pStack)
{
	// synchronously remove the stack from the running strand
	auto action = [this, pContext]()
	{
		pRouter->Remove(pContext);
	};
	pExecutor->BlockFor(action);

	stacks.erase(pStack);

	// post the deletion of the stack to the strand
	auto deleteStack = [pStack]()
	{
		delete pStack;
	};
	pExecutor->strand.post(deleteStack);
}

}


