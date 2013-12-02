
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

#include "SlaveTestObject.h"

#include "BufferHelpers.h"

#include <openpal/ToHex.h>
#include <openpal/LoggableMacros.h>

#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

SlaveTestObject::SlaveTestObject(const SlaveConfig& arCfg, LogLevel aLevel, bool aImmediate) :
	log(),
	mMockTimeWriteHandler([this](UTCTimestamp time)
{
	mTimeWrites.push(time);
}),
mts(),
app(Logger(&log, aLevel, "app")),
db(Logger(&log, aLevel, "db")),
slave(Logger(&log, aLevel, "slave"), &app, &mts, &mMockTimeWriteHandler, &db, &cmdHandler, arCfg),
mLogger(Logger(&log, aLevel, "test"))
{
	app.SetUser(&slave);
}

void SlaveTestObject::SendToSlave(const std::string& arData, SequenceInfo aSeq)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs.ToReadOnly());
	mAPDU.Interpret();
	LOG_BLOCK(LogLevel::Interpret, "<= " << mAPDU.ToString());
	slave.OnRequest(mAPDU, aSeq);
}

bool SlaveTestObject::NothingToRead()
{
	if(app.NothingToRead()) return true;
	else {
		ostringstream oss;
		oss << "Expected nothing but outstation wrote: " << Read();
		throw InvalidStateException(LOCATION, oss.str());
	}
}

std::string SlaveTestObject::Read()
{
	mAPDU = app.Read();	
	return toHex(mAPDU.ToReadOnly(), true);
}

}

