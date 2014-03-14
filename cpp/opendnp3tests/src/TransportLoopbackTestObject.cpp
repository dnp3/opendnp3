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

#include <opendnp3/link/LinkRoute.h>

#include <openpal/IPhysicalLayerAsync.h>

#include <asio.hpp>
#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportLoopbackTestObject::TransportLoopbackTestObject(
    asio::io_service* apService,
    IPhysicalLayerAsync* apPhys,
    LinkConfig aCfgA,
    LinkConfig aCfgB,
    LogLevel aLevel,
    bool aImmediate) :

	AsyncTestObjectASIO(apService),
	log(),
	mLogger(Logger(&log, aLevel, "test")),
	mCfgA(aCfgA),
	mCfgB(aCfgB),
	mLinkA(mLogger, apPhys->GetExecutor(), aCfgA),
	mLinkB(mLogger, apPhys->GetExecutor(), aCfgB),
	mTransA(mLogger, apPhys->GetExecutor()),
	mTransB(mLogger, apPhys->GetExecutor()),
	mRouter(mLogger, apPhys, TimeDuration::Seconds(1), TimeDuration::Seconds(1)),
	mUpperA(mLogger),
	mUpperB(mLogger)
{
	LinkRoute routeA(mCfgA.RemoteAddr, mCfgA.LocalAddr);
	LinkRoute routeB(mCfgB.RemoteAddr, mCfgB.LocalAddr);

	mRouter.AddContext(&mLinkA, routeA);
	mRouter.Enable(&mLinkA);
	mRouter.AddContext(&mLinkB, routeB);
	mRouter.Enable(&mLinkB);

	mLinkA.SetUpperLayer(&mTransA);
	mLinkB.SetUpperLayer(&mTransB);

	mLinkA.SetRouter(&mRouter);
	mLinkB.SetRouter(&mRouter);

	mTransA.SetUpperLayer(&mUpperA);
	mTransB.SetUpperLayer(&mUpperB);
}

TransportLoopbackTestObject::~TransportLoopbackTestObject()
{
	mRouter.Shutdown();
	this->GetService()->run();
}

bool TransportLoopbackTestObject::LayersUp()
{
	return mUpperA.IsLowerLayerUp() && mUpperB.IsLowerLayerUp();
}

void TransportLoopbackTestObject::Start()
{
	mRouter.Start();
}

}


