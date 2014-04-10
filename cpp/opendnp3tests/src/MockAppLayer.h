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
#ifndef __MOCK_APP_LAYER_H_
#define __MOCK_APP_LAYER_H_

#include <opendnp3/app/IAppLayer.h>
#include <opendnp3/app/APDUWrapper.h>
#include <opendnp3/app/IAppUser.h>
#include <opendnp3/app/IAppLayer.h>

#include <openpal/Logger.h>

#include <queue>
#include <string>

namespace opendnp3
{

/**	@section desc Test class to mock async app layer for master/outstation */
class MockAppLayer : public IAppLayer
{
public:
	MockAppLayer(const openpal::Logger&);
	virtual ~MockAppLayer() {}

	void SetUser(IAppUser*);

	void SendResponse(APDUResponse&);
	void SendUnsolicited(APDUResponse&);
	void SendRequest(APDURequest&);
	void CancelResponse();

	bool NothingToRead();

	size_t mNumCancel;

	void EnableAutoSendCallback(bool aIsSuccess);
	void DisableAutoSendCallback();

	std::string Read();

	size_t Count()
	{
		return mFragments.size();
	}

	size_t NumAPDU()
	{
		return mFragments.size();
	}

private:

	void DoSendUnsol();
	void DoSendSol();

	openpal::Logger logger;
	IAppUser* mpUser;
	bool mAutoSendCallback;
	bool mIsSuccess;
	std::deque<std::string> mFragments;
};

}

#endif

