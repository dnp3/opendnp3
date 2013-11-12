
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
#ifndef __SOLICITED_CHANNEL_H_
#define __SOLICITED_CHANNEL_H_

#include "AppLayerChannel.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

/**  The application layer contains two virtual channels, one for
	 solicited and unsolicited communication. Each channel has a sequence
	 number and some state associated with wether it is sending, waiting
	 for a response, etc
*/
class DLL_LOCAL SolicitedChannel : public AppLayerChannel
{
public:
	SolicitedChannel(openpal::Logger aLogger, AppLayer* apApp, openpal::IExecutor* apExecutor, openpal::millis_t aTimeout);
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

#endif

