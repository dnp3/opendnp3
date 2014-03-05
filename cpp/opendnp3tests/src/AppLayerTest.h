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
#ifndef __APP_LAYER_TEST_H_
#define __APP_LAYER_TEST_H_

#include <opendnp3/app/AppLayer.h>

#include "LogTester.h"
#include "MockLowerLayer.h"
#include "MockExecutor.h"

#include "MockAppUser.h"


namespace opendnp3
{


class AppLayerTest
{
public:
	AppLayerTest(bool aIsMaster = false, size_t aNumRetry = 0, LogLevel aLevel = LogLevel::Warning, bool aImmediate = false);

	void SendUp(const std::string& aBytes);
	void SendUp(AppControlField control, FunctionCode aCode);
	void SendUp(AppControlField control, FunctionCode aCode, IINField iin);

	void SendRequest(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);
	void SendResponse(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);
	void SendUnsolicited(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);

	// bool CheckSentAPDU(FunctionCode aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ);
	
	LogTester log;
	MockAppUser user;
	MockLowerLayer lower;
	MockExecutor mts;
	AppLayer app;

	MockAppUser::State state;

	private:

	//bool CheckSentAPDUWithSize(FunctionCode aCode, const AppControlField& acf, uint32_t size);	

	uint8_t buffer[4];
	openpal::WriteBuffer writeBuffer;

};

}

#endif
