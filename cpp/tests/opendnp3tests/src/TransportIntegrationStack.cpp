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
#include "TransportIntegrationStack.h"

#include <opendnp3/Route.h>
#include <opendnp3/app/AppConstants.h>

#include <openpal/channel/IPhysicalLayer.h>

using namespace openpal;

namespace opendnp3
{

TransportIntegrationStack::TransportIntegrationStack(openpal::LogRoot& root, openpal::IExecutor& executor, IPhysicalLayer* apPhys, LinkConfig aCfg) :
	router(root, executor, apPhys, TimeDuration::Seconds(1), TimeDuration::Seconds(1)),
	link(root, &executor, aCfg),
	transport(root, &executor, DEFAULT_MAX_APDU_SIZE)
{
	Route route(aCfg.RemoteAddr, aCfg.LocalAddr);
	router.AddContext(&link, route);
	router.Enable(&link);
	link.SetRouter(router);

	link.SetUpperLayer(transport);
	transport.SetLinkLayer(&link);

	transport.SetAppLayer(&upper);
	upper.SetLowerLayer(transport);
}


}
