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
#include "MockResponseAcceptor.h"


namespace apl
{

void MockResponseAcceptor::AcceptResponse(const CommandResponse& aResponse, int aSequence)
{
	std::unique_lock<std::mutex> lock(mMutex);
	MockResponse mr(aResponse, aSequence);
	mResponses.push_back(mr);
	mCondition.notify_all();
}

MockResponse MockResponseAcceptor::PopResponse()
{
	std::unique_lock<std::mutex> lock(mMutex);
	//if(mResponse.size() == 0) throw ArgumentException(LOCATION,
	MockResponse mr = mResponses.front();
	mResponses.pop_front();
	return mr;
}

size_t MockResponseAcceptor::NumResponses()
{
	std::unique_lock<std::mutex> lock(mMutex);
	return mResponses.size();
}
}
