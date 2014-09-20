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
#include "TransportLoopbackTestObject.h"

#include <opendnp3/Route.h>
#include <opendnp3/app/AppConstants.h>

#include <openpal/channel/IPhysicalLayer.h>

#include <asio.hpp>
#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportLoopbackTestObject::TransportLoopbackTestObject(
	openpal::LogRoot& root,
	asio::io_service& service,
	openpal::IExecutor& executor,
    IPhysicalLayer* apPhys,
    LinkConfig aCfgA,
    LinkConfig aCfgB,
    uint32_t filters) :

	TestObjectASIO(service),
	mCfgA(aCfgA),
	mCfgB(aCfgB),
	mLinkA(root, &executor, aCfgA),
	mLinkB(root, &executor, aCfgB),
	mTransA(root, &executor, DEFAULT_MAX_APDU_SIZE),
	mTransB(root, &executor, DEFAULT_MAX_APDU_SIZE),
	mRouter(root, executor, apPhys, TimeDuration::Seconds(1), TimeDuration::Seconds(1))
{
	Route routeA(mCfgA.RemoteAddr, mCfgA.LocalAddr);
	Route routeB(mCfgB.RemoteAddr, mCfgB.LocalAddr);

	mRouter.AddContext(&mLinkA, routeA);
	mRouter.Enable(&mLinkA);
	mRouter.AddContext(&mLinkB, routeB);
	mRouter.Enable(&mLinkB);

	mLinkA.SetUpperLayer(mTransA);
	mTransA.SetLinkLayer(&mLinkA);

	mLinkB.SetUpperLayer(mTransB);
	mTransB.SetLinkLayer(&mLinkB);

	mLinkA.SetRouter(mRouter);
	mLinkB.SetRouter(mRouter);

	mTransA.SetAppLayer(&mUpperA);
	mTransB.SetAppLayer(&mUpperB);

	mUpperA.SetLowerLayer(mTransA);
	mUpperB.SetLowerLayer(mTransB);
}

TransportLoopbackTestObject::~TransportLoopbackTestObject()
{
	mRouter.Shutdown();
	this->GetService().run();
}

bool TransportLoopbackTestObject::LayersUp()
{
	return mUpperA.IsOnline() && mUpperB.IsOnline();
}

void TransportLoopbackTestObject::Start()
{
	mRouter.Start();
}

}


