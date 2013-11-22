
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
#ifndef __PRI_LINK_LAYER_STATES_H_
#define __PRI_LINK_LAYER_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <openpal/Location.h>

#include "LinkLayer.h"

namespace opendnp3
{

class LinkLayer;

class DLL_LOCAL PriStateBase
{
public:

	/* Incoming messages for primary station */
	virtual void Ack(LinkLayer*, bool aIsRcvBuffFull);
	virtual void Nack(LinkLayer*, bool aIsRcvBuffFull);
	virtual void LinkStatus(LinkLayer*, bool aIsRcvBuffFull);
	virtual void NotSupported (LinkLayer*, bool aIsRcvBuffFull);

	virtual void OnTimeout(LinkLayer*);

	/*Upper layer events to handle */
	virtual void SendConfirmed(LinkLayer*, const uint8_t*, size_t);
	virtual void SendUnconfirmed(LinkLayer*, const uint8_t*, size_t);

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	//every concrete state implements this for logging purposes
	virtual std::string Name() const = 0;
#endif

};

//	@section desc Entry state for primary station
class DLL_LOCAL PLLS_SecNotReset : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SecNotReset);
	void SendUnconfirmed(LinkLayer*, const uint8_t*, size_t);
	void SendConfirmed(LinkLayer*, const uint8_t*, size_t);
};

//	@section desc for reset state
class DLL_LOCAL PLLS_SecReset : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SecReset);
	void SendUnconfirmed(LinkLayer*, const uint8_t*, size_t);
	void SendConfirmed(LinkLayer*, const uint8_t*, size_t);
};

//	@section desc As soon as we get an ACK, send the delayed pri frame
class DLL_LOCAL PLLS_ResetLinkWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ResetLinkWait);

	void Ack(LinkLayer*, bool aIsRcvBuffFull);
	void Nack(LinkLayer*  apLL, bool) {
		Failure(apLL);
	}
	void LinkStatus(LinkLayer* apLL, bool) {
		Failure(apLL);
	}
	void NotSupported (LinkLayer*  apLL, bool) {
		Failure(apLL);
	}

	void OnTimeout(LinkLayer*);

private:
	void Failure(LinkLayer*);
};

//	@section desc As soon as we get an ACK, send the delayed pri frame
class DLL_LOCAL PLLS_ConfDataWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ConfDataWait);

	void Ack(LinkLayer*, bool aIsRcvBuffFull);
	void Nack(LinkLayer* apLL, bool);
	void LinkStatus(LinkLayer* apLL, bool) {
		Failure(apLL);
	}
	void NotSupported (LinkLayer* apLL, bool) {
		Failure(apLL);
	}
	void OnTimeout(LinkLayer*);

private:
	void Failure(LinkLayer*);
};

} //end namepsace

#endif

