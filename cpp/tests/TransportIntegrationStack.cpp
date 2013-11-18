
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

#include "TransportIntegrationStack.h"

#include <opendnp3/LinkRoute.h>

#include <openpal/IPhysicalLayerAsync.h>

using namespace openpal;

namespace opendnp3
{

TransportIntegrationStack::TransportIntegrationStack(Logger aLogger, IPhysicalLayerAsync* apPhys, LinkConfig aCfg) :
	mRouter(aLogger, apPhys, TimeDuration::Seconds(1)),
	mLink(aLogger, apPhys->GetExecutor(), aCfg),
	mTransport(aLogger),
	mUpper(aLogger)
{
	LinkRoute route(aCfg.RemoteAddr, aCfg.LocalAddr);
	mRouter.AddContext(&mLink, route);
	mLink.SetUpperLayer(&mTransport);
	mTransport.SetUpperLayer(&mUpper);
	mLink.SetRouter(&mRouter);
}


}
