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
#include "SlaveTestObject.h"

#include "BufferHelpers.h"
#include "Exception.h"

#include <openpal/ToHex.h>
#include <openpal/LoggableMacros.h>

#include <opendnp3/app/APDUHeaderParser.h>

#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

SlaveTestObject::SlaveTestObject(const SlaveConfig& arCfg, const DatabaseTemplate& dbTemplate, uint32_t filters, bool aImmediate) :
	log(),
	mMockTimeWriteHandler([this](UTCTimestamp time)
{
	mTimeWrites.push(time);
}),
mts(),
app(log.GetLogger("app")),
dbBuffers(dbTemplate),
db(dbBuffers.GetFacade()),
slave(log.GetLogger("slave"), &app, &mts, &mMockTimeWriteHandler, &db, &cmdHandler, arCfg),
mLogger(log.GetLogger("test"))
{
	app.SetUser(&slave);
}

void SlaveTestObject::SendToSlave(const std::string& arData, SequenceInfo aSeq)
{
	HexSequence hs(arData);
	APDURecord record;
	if(APDUHeaderParser::ParseRequest(hs.ToReadOnly(), record) != APDUHeaderParser::Result::OK)
	{
		throw Exception("Why are you trying to send bad data?");
	}
	slave.OnRequest(record, aSeq);
}

bool SlaveTestObject::NothingToRead()
{
	if(app.NothingToRead()) return true;
	else
	{
		ostringstream oss;
		oss << "Expected nothing but outstation wrote: " << Read();
		throw InvalidStateException(LOCATION, oss.str());
	}
}

std::string SlaveTestObject::Read()
{
	return app.Read();
}

}

