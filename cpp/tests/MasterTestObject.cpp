
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "MasterTestObject.h"

#include <memory>
#include "BufferHelpers.h"

#include <opendnp3/ToHex.h>

using namespace std;

namespace opendnp3
{

MasterTestObject::MasterTestObject(MasterConfig cfg, FilterLevel aLevel, bool aImmediate) :
	LogTester(aImmediate),
	fake_time(),
	mts(),
	ats(&fake_time),
	fdo(),
	app(mLog.GetLogger(aLevel, "MockAppLayer")),
	master(mLog.GetLogger(aLevel, "master"), cfg, &app, &fdo, ats.CreateNewGroup(&mts), &mts, &fake_time)
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
	mAPDU.Write(hs, hs.Size());
	mAPDU.Interpret();
	if(aFinal) master.OnFinalResponse(mAPDU);
	else master.OnPartialResponse(mAPDU);
}

void MasterTestObject::SendUnsolToMaster(const std::string& arData)
{
	HexSequence hs(arData);
	mAPDU.Reset();
	mAPDU.Write(hs, hs.Size());
	mAPDU.Interpret();
	master.OnUnsolResponse(mAPDU);
}

std::string MasterTestObject::Read()
{
	mAPDU = app.Read();
	std::string hex = toHex(mAPDU.GetBuffer(), mAPDU.Size(), true);
	return hex;
}

} //end ns

