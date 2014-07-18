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
#include "TransportScalabilityTestObject.h"
#include <sstream>

#include <asio.hpp>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportScalabilityTestObject::TransportScalabilityTestObject(
    LinkConfig aClientCfg,
    LinkConfig aServerCfg,
    uint16_t aPortStart,
    uint16_t aNumPair,
    uint32_t filters,
    bool aImmediate) :

	TestObjectASIO(),
	log()
{
	const uint16_t START = aPortStart;
	const uint16_t STOP = START + aNumPair;

	for(uint16_t port = START; port < STOP; ++port)
	{
		ostringstream oss;
		oss << "pair" << port;
		TransportStackPair* pPair = new TransportStackPair(aClientCfg, aServerCfg, log.root, this->GetService(), port);
		mPairs.push_back(pPair);
	}
}

TransportScalabilityTestObject::~TransportScalabilityTestObject()
{
	for(auto pPair : mPairs)
	{
		pPair->mClientStack.router.Shutdown();
		pPair->mServerStack.router.Shutdown();
	}
	this->GetService().run();
	for(auto pPair : mPairs) delete pPair;
}

bool TransportScalabilityTestObject::AllLayersUp()
{
	for(TransportStackPair * pPair : mPairs)
	{
		if(!pPair->BothLayersUp()) return false;
	}

	return true;
}

bool TransportScalabilityTestObject::AllLayerEqual(const openpal::ReadOnlyBuffer& arBuffer)
{
	for(TransportStackPair * pPair : mPairs)
	{
		if(! pPair->mServerStack.upper.BufferEquals(arBuffer)) return false;
		if(! pPair->mClientStack.upper.BufferEquals(arBuffer)) return false;
	}

	return true;
}

bool TransportScalabilityTestObject::AllLayerReceived(size_t aNumBytes)
{
	for(TransportStackPair * pPair : mPairs)
	{
		if(pPair->mServerStack.upper.Size() != aNumBytes) return false;
		if(pPair->mClientStack.upper.Size() != aNumBytes) return false;
	}

	return true;
}

void TransportScalabilityTestObject::SendToAll(const openpal::ReadOnlyBuffer& arBuffer)
{
	for(TransportStackPair * pPair : mPairs)
	{
		pPair->mClientStack.upper.SendDown(arBuffer);
		pPair->mServerStack.upper.SendDown(arBuffer);
	}
}

void TransportScalabilityTestObject::Start()
{
	for(TransportStackPair * pPair : mPairs)
	{
		pPair->Start();
	}
}

}


