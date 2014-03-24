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
#include "MasterTestObject.h"

#include <memory>
#include "BufferHelpers.h"

#include <openpal/ToHex.h>

#include <opendnp3/app/APDUHeaderParser.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{

MasterTestObject::MasterTestObject(MasterConfig cfg, uint32_t filters, bool aImmediate) :
	log(),
	fixedUTC(0),
	mts(),
	group(&mts),
	meas(),
	app(Logger(&log, filters, "MockAppLayer")),
	master(Logger(&log, filters, "master"), cfg, &app, &meas, &group, &mts, &fixedUTC)
{
	app.SetUser(&master);
}

void MasterTestObject::RespondToMaster(const std::string& arData, bool aFinal)
{
	HexSequence hs(arData);
	APDUResponseRecord record;
	if(APDUHeaderParser::ParseResponse(hs.ToReadOnly(), record) != APDUHeaderParser::Result::OK)
	{
		throw Exception("Why are you trying to send bad data?");
	}
	if(aFinal) master.OnFinalResponse(record);
	else master.OnPartialResponse(record);
}

void MasterTestObject::SendUnsolToMaster(const std::string& arData)
{
	HexSequence hs(arData);
	APDUResponseRecord record;
	if(APDUHeaderParser::ParseResponse(hs.ToReadOnly(), record) != APDUHeaderParser::Result::OK)
	{
		throw Exception("Why are you trying to send bad data?");
	}
	master.OnUnsolResponse(record);
}

std::string MasterTestObject::Read()
{
	return app.Read();
}

} //end ns

