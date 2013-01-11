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
#include "SlaveTestObject.h"

#include <APLTestTools/BufferHelpers.h>
#include <APL/ToHex.h>


namespace apl
{
namespace dnp
{

SlaveTestObject::SlaveTestObject(const SlaveConfig& arCfg, FilterLevel aLevel, bool aImmediate) :
	LogTester(aImmediate),
	mts(),
	app(mLog.GetLogger(aLevel, "app")),
	db(mLog.GetLogger(aLevel, "db")),
	cmd_master(&fakeTime, 10000),
	slave(mLog.GetLogger(aLevel, "slave"), &app, &mts, &fakeTime, &db, &cmd_master, arCfg),
	mpLogger(mLog.GetLogger(aLevel, "test"))
{
	app.SetUser(&slave);
}

void SlaveTestObject::SendToSlave(const std::string& arData, SequenceInfo aSeq)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs, hs.Size());
	mAPDU.Interpret();
	LOG_BLOCK(LEV_INTERPRET, "<= " << mAPDU.ToString());
	slave.OnRequest(mAPDU, aSeq);
}

std::string SlaveTestObject::Read()
{
	mAPDU = app.Read();
	std::string hex = toHex(mAPDU.GetBuffer(), mAPDU.Size(), true);
	return hex;
}

}
}

