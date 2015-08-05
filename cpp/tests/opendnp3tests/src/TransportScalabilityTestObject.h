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
#ifndef __TRANSPORT_SCALABILITY_TEST_OBJECT_H_
#define __TRANSPORT_SCALABILITY_TEST_OBJECT_H_

#include "TransportStackPair.h"
#include "TestObjectASIO.h"
#include <testlib/MockLogHandler.h>

#include <opendnp3/LogLevels.h>
#include <asiopal/ASIOExecutor.h>

namespace opendnp3
{

class TransportScalabilityTestObject : public TestObjectASIO
{
public:
	TransportScalabilityTestObject(
	    LinkConfig aClientCfg,
	    LinkConfig aServerCfg,
	    uint16_t aPortStart,
	    uint16_t aNumPair,
	    uint32_t filters = flags::INFO,
	    bool aImmediate = false);

	~TransportScalabilityTestObject();

	void Start();


	// Test helpers
	bool AllLayersUp();
	bool AllLayerReceived(size_t aNumBytes);
	bool AllLayerEqual(const openpal::RSlice& arBuffer);

	void SendToAll(const openpal::RSlice& arBuffer);

	testlib::MockLogHandler log;
	std::vector<TransportStackPair*> mPairs;
};

}

#endif
