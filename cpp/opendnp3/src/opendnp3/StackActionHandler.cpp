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

#include <openpal/IExecutor.h>

#include "opendnp3/link/LinkLayerRouter.h"

namespace opendnp3
{

StackActionHandler::StackActionHandler(LinkLayerRouter* pRouter_, const LinkRoute& route_, openpal::IExecutor* pExecutor_, openpal::ITypedShutdownHandler<DNP3Stack*>* pHandler_) :
pRouter(pRouter_),
route(route_),
pExecutor(pExecutor_),
pHandler(pHandler_)
{
	
}

void StackActionHandler::EnableRoute()
{
	pExecutor->Post([this](){ pRouter->EnableRoute(route); });
}

void StackActionHandler::DisableRoute()
{
	pExecutor->Post([this](){ pRouter->DisableRoute(route); });
}

void StackActionHandler::ExternalShutdown(DNP3Stack* pStack)
{
	{
		openpal::ExecutorPause pause(pExecutor);
		pRouter->RemoveContext(route);
	}	
	pExecutor->Post([this, pStack](){ pHandler->OnShutdown(pStack); });
}

// already on the executor
void StackActionHandler::InternalShutdown(DNP3Stack* pStack)
{		
	pRouter->RemoveContext(route);
	pHandler->OnShutdown(pStack);
}

}


