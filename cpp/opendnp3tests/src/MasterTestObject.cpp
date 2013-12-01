
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

#include "MasterTestObject.h"

#include <memory>
#include "BufferHelpers.h"

#include <openpal/ToHex.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{

MasterTestObject::MasterTestObject(MasterConfig cfg, openpal::FilterLevel aLevel, bool aImmediate) :
	log(),
	fixedUTC(0),
	mts(),
	group(&mts),
	meas(),
	app(Logger(&log, aLevel, "MockAppLayer")),
	master(Logger(&log, aLevel, "master"), cfg, &app, &meas, &group, &mts, &fixedUTC)
{
	app.SetUser(&master);
}

void MasterTestObject::BindStateListener()
{
	master.AddStateListener([&](StackState state) {
		states.push_back(state);
	});
}

void MasterTestObject::RespondToMaster(const std::string& arData, bool aFinal)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs.ToReadOnly());
	mAPDU.Interpret();
	if(aFinal) master.OnFinalResponse(mAPDU);
	else master.OnPartialResponse(mAPDU);
}

void MasterTestObject::SendUnsolToMaster(const std::string& arData)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs.ToReadOnly());
	mAPDU.Interpret();
	master.OnUnsolResponse(mAPDU);
}

std::string MasterTestObject::Read()
{
	mAPDU = app.Read();
	std::string hex = toHex(mAPDU.ToReadOnly(), true);
	return hex;
}

} //end ns

