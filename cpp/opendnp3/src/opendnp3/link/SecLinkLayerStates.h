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
#ifndef __SEC_LINK_LAYER_STATES_H_
#define __SEC_LINK_LAYER_STATES_H_

#include <string>

#include "opendnp3/Singleton.h"
#include "opendnp3/link/LinkLayer.h"

#include <openpal/LoggableMacros.h>

namespace opendnp3
{

class SecStateBase
{
public:

	/* Incoming messages to secondary station */

	virtual void ResetLinkStates(LinkLayer*) = 0;
	virtual void RequestLinkStatus(LinkLayer*) = 0;	

	virtual void TestLinkStatus(LinkLayer*, bool aFcb) = 0;
	virtual void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&) = 0;

	virtual void OnTransmitResult(LinkLayer* apLL, bool success);

	//every concrete state implements this for logging purposes
	virtual std::string Name() const = 0;
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
	
	virtual void OnTransmitResult(LinkLayer* apLL, bool success) override final;

	virtual void ResetLinkStates(LinkLayer*) override final;
	virtual void RequestLinkStatus(LinkLayer*) override final;
	virtual void TestLinkStatus(LinkLayer*, bool aFcb) override final;
	virtual void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&) override final;
};


template <class NextState>
void SLLS_TransmitWaitBase<NextState>::OnTransmitResult(LinkLayer* apLL, bool success)
{	
	// with secondary replies, we dont' really care about success
	apLL->ChangeState(NextState::Inst());
}

template <class NextState>
void SLLS_TransmitWaitBase<NextState>::ResetLinkStates(LinkLayer* apLL)
{
	LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Ignoring link frame, remote is flooding");
}

template <class NextState>
void SLLS_TransmitWaitBase<NextState>::RequestLinkStatus(LinkLayer* apLL)
{
	LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Ignoring link frame, remote is flooding");
}

template <class NextState>
void SLLS_TransmitWaitBase<NextState>::TestLinkStatus(LinkLayer* apLL, bool aFcb)
{
	LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Ignoring link frame, remote is flooding");
}

template <class NextState>
void SLLS_TransmitWaitBase<NextState>::ConfirmedUserData(LinkLayer* apLL, bool aFcb, const openpal::ReadOnlyBuffer&)
{
	LOGGER_BLOCK(apLL->GetLogger(), LogLevel::Warning, "Ignoring link frame, remote is flooding");
}

////////////////////////////////////////////////////////
//	Class SLLS_UnReset
////////////////////////////////////////////////////////
class SLLS_NotReset : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_NotReset);
	
	virtual void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&) override final;

	virtual void ResetLinkStates(LinkLayer*) override final;
	virtual void RequestLinkStatus(LinkLayer*) override final;
	virtual void TestLinkStatus(LinkLayer*, bool aFcb) override final;
};

////////////////////////////////////////////////////////
//	Class SLLS_Reset
////////////////////////////////////////////////////////
class SLLS_Reset : public SecStateBase
{
	MACRO_STATE_SINGLETON_INSTANCE(SLLS_Reset);

	virtual void ConfirmedUserData(LinkLayer*, bool aFcb, const openpal::ReadOnlyBuffer&) override final;
	
	virtual void ResetLinkStates(LinkLayer*) override final;
	virtual void RequestLinkStatus(LinkLayer*) override final;
	virtual void TestLinkStatus(LinkLayer*, bool aFcb) override final;
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

