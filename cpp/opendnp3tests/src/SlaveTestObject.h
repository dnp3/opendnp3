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
#ifndef __SLAVE_TEST_OBJECT_H_
#define __SLAVE_TEST_OBJECT_H_

#include <opendnp3/Slave.h>
#include <opendnp3/Database.h>
#include <opendnp3/SlaveConfig.h>
#include <opendnp3/DynamicallyAllocatedDatabase.h>

#include "MockExecutor.h"
#include "LogTester.h"
#include "MockCommandHandler.h"
#include "MockAppLayer.h"
#include "MockTimeWriteHandler.h"

namespace opendnp3
{

class SlaveTestObject
{
public:
	SlaveTestObject(const SlaveConfig& arCfg, const DatabaseTemplate& dbTemplate, openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);

	void SendToSlave(const std::string& arData, SequenceInfo aSeq = SequenceInfo::OTHER);

	std::string Read();

	bool NothingToRead();

	size_t Count() {
		return app.Count();
	}


	LogTester log;
	MockTimeWriteHandler mMockTimeWriteHandler;
	MockExecutor mts;
	MockAppLayer app;
	DynamicallyAllocatedDatabase dbBuffers;
	Database db;
	MockCommandHandler cmdHandler;
	Slave slave;	
	openpal::Logger mLogger;

	std::queue<openpal::UTCTimestamp> mTimeWrites;

};

}

#endif

