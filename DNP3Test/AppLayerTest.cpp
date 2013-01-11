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
#include "AppLayerTest.h"

#include <APLTestTools/BufferHelpers.h>

namespace apl
{
namespace dnp
{

AppLayerTest::AppLayerTest(bool aIsMaster, size_t aNumRetry, FilterLevel aLevel, bool aImmediate) :
	LogTester(aImmediate),
	user(aIsMaster),
	lower(mLog.GetLogger(aLevel, "lower")),
	mts(),
	app(mLog.GetLogger(aLevel, "app"), &mts, AppConfig(1000, aNumRetry))
{
	lower.SetUpperLayer(&app);
	app.SetUser(&user);
}

void AppLayerTest::SendUp(const std::string& aBytes)
{
	HexSequence hs(aBytes);
	lower.SendUp(hs, hs.Size());
}

void AppLayerTest::SendUp(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ)
{
	APDU f;
	f.SetFunction(aCode);
	f.SetControl(aFIR, aFIN, aCON, aUNS, aSEQ);
	lower.SendUp(f.GetBuffer(), f.Size());
}

void AppLayerTest::SendRequest(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);

	app.SendRequest(mFragment);
}

void AppLayerTest::SendResponse(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);
	if(aCode == FC_RESPONSE) { //write a NULL IIN so that the buffers will match
		IINField iin;
		mFragment.SetIIN(iin);
	}
	app.SendResponse(mFragment);
}

void AppLayerTest::SendUnsolicited(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);
	if(aCode == FC_UNSOLICITED_RESPONSE) { //write a NULL IIN so that the buffers will match
		IINField iin;
		mFragment.SetIIN(iin);
	}
	app.SendUnsolicited(mFragment);
}

bool AppLayerTest::CheckSentAPDU(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ)
{
	APDU f;
	f.SetFunction(aCode);
	f.SetControl(aFIR, aFIN, aCON, aUNS, aSEQ);
	if(aCode == FC_UNSOLICITED_RESPONSE || aCode == FC_RESPONSE) {
		IINField iin;
		f.SetIIN(iin);
	}
	bool ret = lower.BufferEquals(f.GetBuffer(), f.Size());
	if(ret) lower.ClearBuffer();
	return ret;
}

}
}
