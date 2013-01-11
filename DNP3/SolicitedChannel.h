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
#ifndef __SOLICITED_CHANNEL_H_
#define __SOLICITED_CHANNEL_H_

#include "AppLayerChannel.h"

namespace apl
{
namespace dnp
{

/**  The application layer contains two virtual channels, one for
	 solicited and unsolicited communication. Each channel has a sequence
	 number and some state associated with wether it is sending, waiting
	 for a response, etc
*/
class SolicitedChannel : public AppLayerChannel
{
public:
	SolicitedChannel(Logger* apLogger, AppLayer* apApp, IExecutor* apExecutor, millis_t aTimeout);
	virtual ~SolicitedChannel() {}

	// Called when the app layer has a problem parsing an object header
	void OnUnknownObjectInRequest(const AppControlField& acf);
	void OnResponse(APDU& arAPDU);
	void OnRequest(APDU& arAPDU);

	bool AcceptsResponse();

private:

	// implement virtual memebers from AppLayerChannel
	void DoSendSuccess();
	void DoFailure();
};

}
}

#endif
