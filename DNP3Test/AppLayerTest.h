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

#ifndef __APP_LAYER_TEST_H_
#define __APP_LAYER_TEST_H_

#include <APLTestTools/LogTester.h>
#include <DNP3/AppLayer.h>
#include <APLTestTools/MockLowerLayer.h>
#include <APLTestTools/MockExecutor.h>

#include "MockAppUser.h"

namespace apl
{
namespace dnp
{


class AppLayerTest : public LogTester
{
public:
	AppLayerTest(bool aIsMaster = false, size_t aNumRetry = 0, FilterLevel aLevel = LEV_WARNING, bool aImmediate = false);

	void SendUp(const std::string& aBytes);
	void SendUp(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ);

	void SendRequest(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);
	void SendResponse(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);
	void SendUnsolicited(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS);

	bool CheckSentAPDU(FunctionCodes aCode, bool aFIR, bool aFIN, bool aCON, bool aUNS, int aSEQ);

	//bool CheckState() { return user.Equals(state); }

	MockAppUser user;
	MockLowerLayer lower;
	MockExecutor mts;
	AppLayer app;

	MockAppUser::State state;

private:
	APDU mFragment;
};

}
}

#endif
