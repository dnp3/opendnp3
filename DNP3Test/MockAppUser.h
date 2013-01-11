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
#ifndef __MOCK_APP_USER_H_
#define __MOCK_APP_USER_H_

#include <DNP3/AppInterfaces.h>

#include <sstream>

namespace apl
{
namespace dnp
{

// @section desc Test class for app layer
class MockAppUser : public IAppUser
{
public:

	struct State {
		friend std::ostream& operator<<(std::ostream& output, const State& s);

		State();

		bool operator==(const State& arState) const;

		size_t NumLayerUp;
		size_t NumLayerDown;
		size_t NumUnsol;
		size_t NumSolSendSuccess;
		size_t NumSolFailure;
		size_t NumUnsolSendSuccess;
		size_t NumUnsolFailure;
		size_t NumPartialRsp;
		size_t NumFinalRsp;
		size_t NumRequest;
		size_t NumUnknown;
	};

	MockAppUser(bool aIsMaster);

	// Implement IAppUser
	void OnLowerLayerUp();
	void OnLowerLayerDown();

	void OnSolSendSuccess();
	void OnSolFailure();

	void OnUnsolSendSuccess();
	void OnUnsolFailure();

	bool IsMaster();
	void OnPartialResponse(const APDU&);
	void OnFinalResponse(const APDU&);
	void OnUnsolResponse(const APDU&);
	void OnRequest(const APDU&, SequenceInfo);
	void OnUnknownObject();

	bool Equals(const State& arState) const;

private:

	bool mIsMaster;

public:
	State mState;
};

}
}

#endif
