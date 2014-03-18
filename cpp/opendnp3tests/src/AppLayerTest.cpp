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
	app(Logger(&log, aLevel, "app"), &mts, AppConfig(aIsMaster, TimeDuration::Seconds(1), aNumRetry)),
	writeBuffer(buffer, 4)
{
	lower.SetUpperLayer(&app);
	app.SetLowerLayer(&lower);
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
	APDURequest out(writeBuffer.Truncate(2));
	out.SetFunction(aCode);
	out.SetControl(AppControlField(aFIR, aFIN, aCON, aUNS));
	app.SendRequest(out);

}

void AppLayerTest::SendResponse(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	APDUResponse out(writeBuffer);
	out.SetFunction(aCode);
	out.SetControl(AppControlField(aFIR, aFIN, aCON, aUNS));
	out.SetIIN(IINField::Empty);
	app.SendResponse(out);
}

void AppLayerTest::SendUnsolicited(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS)
{
	APDUResponse out(writeBuffer);
	out.SetFunction(aCode);
	out.SetControl(AppControlField(aFIR, aFIN, aCON, aUNS));
	out.SetIIN(IINField::Empty);
	app.SendUnsolicited(out);
}

/*
bool AppLayerTest::CheckSentAPDU(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ)
{
	AppControlField acf(aFIR, aFIN, aCON, aUNS, aSEQ);
	switch(aCode)
	{
		case(FunctionCode::RESPONSE):
		case(FunctionCode::UNSOLICITED_RESPONSE):
			return CheckSentAPDUWithSize(aCode, acf, 4);
		default:
			return CheckSentAPDUWithSize(aCode, acf, 2);
	}
}
*/

/*
bool AppLayerTest::CheckSentAPDUWithSize(FunctionCode aCode, const AppControlField& acf, uint32_t size)
{
	uint8_t buffer[4];
	assert(size == 2 || size == 4);
	openpal::WriteBuffer wb(buffer, size);
	APDUWrapper apdu(wb);
	apdu.SetFunction(aCode);
	apdu.SetControl(acf);
	bool ret = lower.BufferEquals(apdu.ToReadOnly());
	if(ret) lower.ClearBuffer();
	return ret;
}
*/



}

