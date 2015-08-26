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
#ifndef OPENDNP3_SECLINKLAYERSTATES_H
#define OPENDNP3_SECLINKLAYERSTATES_H

#include "opendnp3/LogLevels.h"
#include "opendnp3/link/Singleton.h"
#include "opendnp3/link/LinkLayer.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

class SecStateBase
{
public:

	// Incoming messages to secondary station

	virtual SecStateBase& OnResetLinkStates(LinkLayer&) = 0;
	virtual SecStateBase& OnRequestLinkStatus(LinkLayer&) = 0;

	virtual SecStateBase& OnTestLinkStatus(LinkLayer&, bool aFcb) = 0;
	virtual SecStateBase& OnConfirmedUserData(LinkLayer&, bool aFcb, const openpal::RSlice&) = 0;

	virtual SecStateBase& OnTransmitResult(LinkLayer& link, bool success);

	//every concrete state implements this for logging purposes

	virtual char const* Name() const = 0;
};

////////////////////////////////////////////////////////
//	Class SLLS_TransmitWait
////////////////////////////////////////////////////////
template <class NextState>
class SLLS_TransmitWaitBase : public SecStateBase
{

protected:
	SLLS_TransmitWaitBase()
	{}

public:

	virtual SecStateBase& OnTransmitResult(LinkLayer& link, bool success) override final;
	virtual SecStateBase& OnResetLinkStates(LinkLayer&) override final;
	virtual SecStateBase& OnRequestLinkStatus(LinkLayer&) override final;
	virtual SecStateBase& OnTestLinkStatus(LinkLayer&, bool aFcb) override final;
	virtual SecStateBase& OnConfirmedUserData(LinkLayer&, bool aFcb, const openpal::RSlice&) override final;
};


template <class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnTransmitResult(LinkLayer& link, bool success)
{
	// with secondary replies, we dont' really care about success
	return NextState::Instance();
}

template <class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnResetLinkStates(LinkLayer& link)
{
	SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
	return *this;
}

template <class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnRequestLinkStatus(LinkLayer& link)
{
	SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
	return *this;
}

template <class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnTestLinkStatus(LinkLayer& link, bool aFcb)
{
	SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
	return *this;
}

template <class NextState>
SecStateBase& SLLS_TransmitWaitBase<NextState>::OnConfirmedUserData(LinkLayer& link, bool aFcb, const openpal::RSlice&)
{
	SIMPLE_LOG_BLOCK(link.ctx.logger, flags::WARN, "Ignoring link frame, remote is flooding");
	return *this;
}

////////////////////////////////////////////////////////
//	Class SLLS_UnReset
////////////////////////////////////////////////////////
class SLLS_NotReset final : public SecStateBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(SLLS_NotReset);	

	virtual SecStateBase& OnConfirmedUserData(LinkLayer&, bool fcb, const openpal::RSlice&) override;
	virtual SecStateBase& OnResetLinkStates(LinkLayer&) override;
	virtual SecStateBase& OnRequestLinkStatus(LinkLayer&) override;
	virtual SecStateBase& OnTestLinkStatus(LinkLayer&, bool fcb) override;

};

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
class SLLS_Reset final : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_Reset);

	virtual SecStateBase& OnConfirmedUserData(LinkLayer&, bool aFcb, const openpal::RSlice&) override;
	virtual SecStateBase& OnResetLinkStates(LinkLayer&) override;
	virtual SecStateBase& OnRequestLinkStatus(LinkLayer&) override;
	virtual SecStateBase& OnTestLinkStatus(LinkLayer&, bool aFcb) override;
};


class SLLS_TransmitWaitReset : public SLLS_TransmitWaitBase<SLLS_Reset>
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_TransmitWaitReset);
};

class SLLS_TransmitWaitNotReset : public SLLS_TransmitWaitBase<SLLS_NotReset>
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_TransmitWaitNotReset);
};


} //end namepsace

#endif

