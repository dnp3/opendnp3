
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

#ifndef __MOCK_APP_LAYER_H_
#define __MOCK_APP_LAYER_H_

#include <opendnp3/AppInterfaces.h>
#include <opendnp3/APDUConstants.h>
#include <opendnp3/APDU.h>
#include <opendnp3/Loggable.h>

#include <queue>

namespace opendnp3
{

class Logger;

/**	@section desc Test class to mock async app layer for master/slave */
class MockAppLayer : public IAppLayer, public Loggable
{
public:
	MockAppLayer(Logger*);
	virtual ~MockAppLayer() {}


	void SetUser(IAppUser*);

	void SendResponse(APDU&);
	void SendUnsolicited(APDU&);
	void SendRequest(APDU&);
	void CancelResponse();
	void CancelUnsolicitedRetries();

	bool NothingToRead();

	size_t mNumCancel;
	size_t mNumCancelUnsolcited;

	void EnableAutoSendCallback(bool aIsSuccess);
	void DisableAutoSendCallback();

	APDU Read();
	size_t Count() {
		return mFragments.size();
	}
	FunctionCodes ReadFunction();
	size_t NumAPDU() {
		return mFragments.size();
	}

private:

	void DoSendUnsol();
	void DoSendSol();

	IAppUser* mpUser;
	bool mAutoSendCallback;
	bool mIsSuccess;
	std::deque<APDU> mFragments;
};

}

#endif

