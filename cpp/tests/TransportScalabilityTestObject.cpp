
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

#include "TransportScalabilityTestObject.h"
#include <sstream>

#include <boost/asio.hpp>

using namespace std;

namespace opendnp3
{

TransportScalabilityTestObject::TransportScalabilityTestObject(
        LinkConfig aClientCfg,
        LinkConfig aServerCfg,
        boost::uint16_t aPortStart,
        boost::uint16_t aNumPair,
        FilterLevel aLevel,
        bool aImmediate) :

	LogTester(aImmediate),
	AsyncTestObjectASIO(),
	mpLogger(mLog.GetLogger(aLevel, "test"))
{
	const boost::uint16_t START = aPortStart;
	const boost::uint16_t STOP = START + aNumPair;

	for(boost::uint16_t port = START; port < STOP; ++port) {
		ostringstream oss;
		oss << "pair" << port;
		Logger* pLogger = mpLogger->GetSubLogger(oss.str());
		TransportStackPair* pPair = new TransportStackPair(aClientCfg, aServerCfg, pLogger, this->GetService(), port);
		mPairs.push_back(pPair);
	}
}

TransportScalabilityTestObject::~TransportScalabilityTestObject()
{
for(auto pPair: mPairs) {
		pPair->mClientStack.mRouter.Shutdown();
		pPair->mServerStack.mRouter.Shutdown();
	}
	this->GetService()->run();
for(auto pPair: mPairs) delete pPair;
}

bool TransportScalabilityTestObject::AllLayersUp()
{
for(TransportStackPair * pPair: mPairs) {
		if(!pPair->BothLayersUp()) return false;
	}

	return true;
}

bool TransportScalabilityTestObject::AllLayerEqual(const uint8_t* apData, size_t aNumBytes)
{
for(TransportStackPair * pPair: mPairs) {
		if(! pPair->mServerStack.mUpper.BufferEquals(apData, aNumBytes)) return false;
		if(! pPair->mClientStack.mUpper.BufferEquals(apData, aNumBytes)) return false;
	}

	return true;
}

bool TransportScalabilityTestObject::AllLayerReceived(size_t aNumBytes)
{
for(TransportStackPair * pPair: mPairs) {
		if(pPair->mServerStack.mUpper.Size() != aNumBytes) return false;
		if(pPair->mClientStack.mUpper.Size() != aNumBytes) return false;
	}

	return true;
}

void TransportScalabilityTestObject::SendToAll(const uint8_t* apData, size_t aNumBytes)
{
for(TransportStackPair * pPair: mPairs) {
		pPair->mClientStack.mUpper.SendDown(apData, aNumBytes);
		pPair->mServerStack.mUpper.SendDown(apData, aNumBytes);
	}
}

void TransportScalabilityTestObject::Start()
{
for(TransportStackPair * pPair: mPairs) {
		pPair->Start();
	}
}

}


