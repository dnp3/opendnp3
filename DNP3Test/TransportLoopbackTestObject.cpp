//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "TransportLoopbackTestObject.h"

#include <DNP3/LinkRoute.h>

#include <sstream>


using namespace std;

namespace apl
{
namespace dnp
{

TransportLoopbackTestObject::TransportLoopbackTestObject(
    boost::asio::io_service* apService,
    IPhysicalLayerAsync* apPhys,
    LinkConfig aCfgA,
    LinkConfig aCfgB,
    FilterLevel aLevel,
    bool aImmediate) :

	LogTester(aImmediate),
	AsyncTestObjectASIO(apService),
	mpLogger(mLog.GetLogger(aLevel, "test")),
	mExecutor(this->GetService()),
	mCfgA(aCfgA),
	mCfgB(aCfgB),
	mLinkA(mpLogger, &mExecutor, aCfgA),
	mLinkB(mpLogger, &mExecutor, aCfgB),
	mTransA(mpLogger),
	mTransB(mpLogger),
	mRouter(mpLogger, apPhys, &mExecutor, 1000),
	mUpperA(mpLogger),
	mUpperB(mpLogger)
{
	mRouter.AddContext(&mLinkA, LinkRoute(mCfgA.RemoteAddr, mCfgA.LocalAddr));
	mRouter.AddContext(&mLinkB, LinkRoute(mCfgB.RemoteAddr, mCfgB.LocalAddr));

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
}


