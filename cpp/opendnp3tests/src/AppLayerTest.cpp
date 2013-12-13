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
#include "AppLayerTest.h"

#include "BufferHelpers.h"

namespace opendnp3
{

AppLayerTest::AppLayerTest(bool aIsMaster, size_t aNumRetry, LogLevel aLevel, bool aImmediate) :
	log(),
	user(Logger(&log, LogLevel::Debug, "user")),
	lower(Logger(&log, aLevel, "lower")),
	mts(),
	app(Logger(&log, aLevel, "app"), &mts, AppConfig(aIsMaster, TimeDuration::Seconds(1), aNumRetry))
{
	lower.SetUpperLayer(&app);
	app.SetUser(&user);
}

void AppLayerTest::SendUp(const std::string& aBytes)
{
	HexSequence hs(aBytes);
	lower.SendUp(hs.ToReadOnly());
}

void AppLayerTest::SendUp(AppControlField control, FunctionCode aCode)
{	
	uint8_t bytes[2] = { control.ToByte(),  FunctionCodeToType(aCode) };
	ReadOnlyBuffer buff(bytes, 2);
	lower.SendUp(buff);
}
	
void AppLayerTest::SendUp(AppControlField control, FunctionCode aCode, IINField iin)
{
	uint8_t bytes[4] = { control.ToByte(),  FunctionCodeToType(aCode), iin.LSB, iin.MSB };
	ReadOnlyBuffer buff(bytes, 4);
	lower.SendUp(buff);
}

void AppLayerTest::SendRequest(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);

	app.SendRequest(mFragment);
}

void AppLayerTest::SendResponse(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);
	if(aCode == FunctionCode::RESPONSE) { //write a nullptr IIN so that the buffers will match
		IINField iin;
		mFragment.SetIIN(iin);
	}
	app.SendResponse(mFragment);
}

void AppLayerTest::SendUnsolicited(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	mFragment.Reset();
	mFragment.SetFunction(aCode);
	mFragment.SetControl(aFIR, aFIN, aCON, aUNS);
	if(aCode == FunctionCode::UNSOLICITED_RESPONSE) { //write a nullptr IIN so that the buffers will match
		IINField iin;
		mFragment.SetIIN(iin);
	}
	app.SendUnsolicited(mFragment);
}

bool AppLayerTest::CheckSentAPDU(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ)
{
	APDU f;
	f.SetFunction(aCode);
	f.SetControl(aFIR, aFIN, aCON, aUNS, aSEQ);
	if(aCode == FunctionCode::UNSOLICITED_RESPONSE || aCode == FunctionCode::RESPONSE) {
		IINField iin;
		f.SetIIN(iin);
	}
	bool ret = lower.BufferEquals(f.ToReadOnly());
	if(ret) lower.ClearBuffer();
	return ret;
}

}

