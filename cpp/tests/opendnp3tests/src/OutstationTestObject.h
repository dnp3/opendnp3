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
#ifndef __OUTSTATION_TEST_OBJECT_H_
#define __OUTSTATION_TEST_OBJECT_H_

#include <opendnp3/LogLevels.h>
#include <opendnp3/app/ITransactable.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/DynamicallyAllocatedDatabase.h>
#include <opendnp3/outstation/DynamicallyAllocatedEventBuffer.h>

#include "MockExecutor.h"
#include "LogTester.h"
#include "MockCommandHandler.h"
#include "MockLowerLayer.h"
#include "MockOutstationApplication.h"

namespace opendnp3
{

class OutstationTestObject
{

public:
	OutstationTestObject(const OutstationConfig& config,
						 const DatabaseTemplate& dbTemplate = DatabaseTemplate());

	
	uint32_t SendToOutstation(const std::string& hex);

	uint32_t LowerLayerUp();

	uint32_t LowerLayerDown();	

	uint32_t OnSendResult(bool isSuccess);

	size_t NumPendingTimers() const;	

	bool AdvanceToNextTimer();

	uint32_t AdvanceTime(const openpal::TimeDuration& td);	

	LogTester log;

	template <class Apply>
	uint32_t Transaction(const Apply& apply)
	{
		{
			opendnp3::Transaction tx(db);
			apply(db);
		}
		return exe.RunMany();
	}

private:

	MockExecutor exe;

public:

	MockLowerLayer lower;

private:

	opendnp3::DynamicallyAllocatedDatabase dbBuffers;
	opendnp3::DynamicallyAllocatedEventBuffer eventBuffers;	

public:
	Database db;
	MockCommandHandler cmdHandler;
	MockOutstationApplication application;

private:
	Outstation outstation;	
};


}

#endif
