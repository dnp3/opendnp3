
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
#ifndef __MOCK_RESPONSE_ACCEPTOR_H_
#define __MOCK_RESPONSE_ACCEPTOR_H_


#include <queue>
#include <APL/CommandTypes.h>
#include <APL/CommandInterfaces.h>
#include <APL/Types.h>

#include <mutex>
#include <condition_variable>

namespace apl
{
struct MockResponse {
	MockResponse(const apl::CommandResponse& aResponse, int aSequence) :
		Response(aResponse),
		Sequence(aSequence)
	{}

	apl::CommandResponse Response;
	int Sequence;
};

class MockResponseAcceptor : public apl::IResponseAcceptor
{
public:

	void AcceptResponse(const apl::CommandResponse& aResponse, int aSequence);

	MockResponse PopResponse();
	size_t NumResponses();

private:
	std::mutex mMutex;
	std::condition_variable mCondition;
	std::deque<MockResponse> mResponses;
};
}

#endif
