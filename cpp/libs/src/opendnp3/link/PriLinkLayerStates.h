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
#ifndef OPENDNP3_PRILINKLAYERSTATES_H
#define OPENDNP3_PRILINKLAYERSTATES_H

#include "opendnp3/link/Singleton.h"
#include "opendnp3/link/LinkLayer.h"

namespace opendnp3
{

class LinkLayer;



class PriStateBase
{
public:

	// Incoming messages for primary station
	virtual PriStateBase& OnAck(LinkLayer&, bool receiveBuffFull);
	virtual PriStateBase& OnNack(LinkLayer&, bool receiveBuffFull);
	virtual PriStateBase& OnLinkStatus(LinkLayer&, bool receiveBuffFull);
	virtual PriStateBase& OnNotSupported(LinkLayer&, bool receiveBuffFull);

	virtual PriStateBase& OnTransmitResult(LinkLayer&, bool success);

	virtual PriStateBase& OnTimeout(LinkLayer&);

	// Upper layer events to handle
	virtual PriStateBase& OnSendConfirmed(LinkLayer&, ITransportSegment& segments);
	virtual PriStateBase& OnSendUnconfirmed(LinkLayer&, ITransportSegment& segments);

	//every concrete state implements this for logging purposes
	virtual char const* Name() const = 0;
};

//	@section desc Entry state for primary station
class PLLS_SecNotReset final : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SecNotReset);

	virtual PriStateBase& OnSendUnconfirmed(LinkLayer&, ITransportSegment& segments) override;
	virtual PriStateBase& OnSendConfirmed(LinkLayer&, ITransportSegment& segments) override;
};


/////////////////////////////////////////////////////////////////////////////
//  template wait state for send unconfirmed sata
/////////////////////////////////////////////////////////////////////////////

template <class ReturnToState>
class PLLS_SendUnconfirmedTransmitWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SendUnconfirmedTransmitWait<ReturnToState>);

	virtual PriStateBase& OnTransmitResult(LinkLayer& link, bool success);
};

template <class ReturnToState>
PLLS_SendUnconfirmedTransmitWait<ReturnToState> PLLS_SendUnconfirmedTransmitWait<ReturnToState>::instance;

template <class ReturnToState>
PriStateBase& PLLS_SendUnconfirmedTransmitWait<ReturnToState>::OnTransmitResult(LinkLayer& link, bool success)
{
	if (link.pSegments->Advance())
	{
		auto output = link.FormatPrimaryBufferWithUnconfirmed(link.pSegments->GetSegment());
		link.QueueTransmit(output, true);
		return *this;
	}
	else // we're done
	{		
		link.DoSendResult(success);
		return ReturnToState::Instance();
	}	
}

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit the reset links
/////////////////////////////////////////////////////////////////////////////

class PLLS_LinkResetTransmitWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_LinkResetTransmitWait);

	virtual PriStateBase& OnTransmitResult(LinkLayer& link, bool success);
};

/////////////////////////////////////////////////////////////////////////////
//  Wait for the link layer to transmit confirmed user data
/////////////////////////////////////////////////////////////////////////////

class PLLS_ConfUserDataTransmitWait : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ConfUserDataTransmitWait);

	virtual PriStateBase& OnTransmitResult(LinkLayer& link, bool success);
};

/////////////////////////////////////////////////////////////////////////////
//  idle state when the secondary is reset
/////////////////////////////////////////////////////////////////////////////

//	@section desc for reset state
class PLLS_SecReset final : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_SecReset);

	virtual PriStateBase& OnSendUnconfirmed(LinkLayer&, ITransportSegment& segments) override;
	virtual PriStateBase& OnSendConfirmed(LinkLayer&, ITransportSegment& segments) override;
};

//	@section desc As soon as we get an ACK, send the delayed pri frame
class PLLS_ResetLinkWait final : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ResetLinkWait);

	PriStateBase& OnAck(LinkLayer&, bool aIsRcvBuffFull) override;

	PriStateBase& OnNack(LinkLayer&  link, bool)  override { return Failure(link); }
	PriStateBase& OnLinkStatus(LinkLayer& link, bool) override { return Failure(link); }
	PriStateBase& OnNotSupported(LinkLayer&  link, bool)  override { return Failure(link); }

	PriStateBase& OnTimeout(LinkLayer&) override;

private:
	PriStateBase& Failure(LinkLayer&);
};

//	@section desc As soon as we get an ACK, send the delayed pri frame
class PLLS_ConfDataWait final : public PriStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(PLLS_ConfDataWait);

	PriStateBase& OnAck(LinkLayer&, bool aIsRcvBuffFull) override;
	PriStateBase& OnNack(LinkLayer& link, bool) override;
	PriStateBase& OnLinkStatus(LinkLayer& link, bool) override { return Failure(link); }
	PriStateBase& OnNotSupported(LinkLayer& link, bool)  override { return Failure(link); }
	
	PriStateBase& OnTimeout(LinkLayer&) override;

private:
	PriStateBase& Failure(LinkLayer&);
};

} //end namepsace

#endif

