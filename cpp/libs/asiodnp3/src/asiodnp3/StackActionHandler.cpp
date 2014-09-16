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

#include "StackActionHandler.h"

#include <openpal/executor/IExecutor.h>
#include <asiopal/ASIOExecutor.h>
#include <asiopal/StrandGetters.h>

#include "asiodnp3/impl/LinkLayerRouter.h"

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

StackActionHandler::StackActionHandler(LinkLayerRouter& router, asiopal::ASIOExecutor& executor) :
	pRouter(&router),
	pExecutor(&executor)	
{

}

asiopal::ASIOExecutor* StackActionHandler::GetExecutor()
{
	return pExecutor;
}

bool StackActionHandler::EnableRoute(ILinkContext* pContext)
{
	auto enable = [this, pContext]() { return pRouter->Enable(pContext); };
	return asiopal::SynchronouslyGet<bool>(pExecutor->strand, enable);
}

bool StackActionHandler::DisableRoute(ILinkContext* pContext)
{
	auto disable = [this, pContext]() { return pRouter->Disable(pContext); };
	return asiopal::SynchronouslyGet<bool>(pExecutor->strand, disable);
}

void StackActionHandler::Shutdown(ILinkContext* pContext)
{
	auto action = [this, pContext](){ pRouter->Remove(pContext); };
	asiopal::SynchronouslyExecute(pExecutor->strand, action);
}

}


