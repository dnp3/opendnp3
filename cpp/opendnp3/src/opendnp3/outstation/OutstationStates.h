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
#ifndef __OUTSTATION_STATES_H_
#define __OUTSTATION_STATES_H_

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUWrapper.h"

#include <openpal/Uncopyable.h>

namespace opendnp3
{

class OutstationContext;

/**
 * Base class for various outstation states
 */
class OutstationSolicitedStateBase
{
public:	

	virtual void OnConfirm(OutstationContext*, const APDUHeader& frag);	

	virtual void OnSendResult(OutstationContext*, bool isSucccess);

	virtual void OnConfirmTimeout(OutstationContext*);

	virtual void OnNewRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality) = 0;

	virtual void OnRepeatRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects) = 0;

protected:

};

class OutstationSolicitedStateIdle : public OutstationSolicitedStateBase, private openpal::Uncopyable
{

public:

	static OutstationSolicitedStateBase& Inst();

	virtual void OnNewRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality) override final;

	virtual void OnRepeatRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects) override final;	

private:

	static OutstationSolicitedStateIdle instance;

	OutstationSolicitedStateIdle() {}

};

/**
* transmitting a response, no confirmation is necessary
*/
class OutstationSolicitedStateTransmitNoConfirm : public OutstationSolicitedStateBase, private openpal::Uncopyable
{
public:

	static OutstationSolicitedStateBase& Inst();

	virtual void OnSendResult(OutstationContext*, bool isSucccess) override final;

	virtual void OnNewRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality) override final;

	virtual void OnRepeatRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects) override final;

private:

	static OutstationSolicitedStateTransmitNoConfirm instance;

	OutstationSolicitedStateTransmitNoConfirm() {}

};

/**
* transmitting a response to a read, confirmation is necessary
*/
class OutstationSolicitedStateTransmitThenConfirm : public OutstationSolicitedStateBase, private openpal::Uncopyable
{
public:

	static OutstationSolicitedStateBase& Inst();

	virtual void OnSendResult(OutstationContext*, bool isSucccess) override final;

	virtual void OnNewRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality) override final;

	virtual void OnRepeatRequest(OutstationContext*, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects) override final;

private:

	static OutstationSolicitedStateTransmitThenConfirm instance;

	OutstationSolicitedStateTransmitThenConfirm() {}
};

/*
* waiting for a confirm to a solicited read response
*/
class OutstationStateSolicitedConfirmWait : public OutstationSolicitedStateBase, private openpal::Uncopyable
{

public:

	static OutstationSolicitedStateBase& Inst();

	virtual void OnNewRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality) override final;

	virtual void OnRepeatRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects) override final;	

	virtual void OnConfirm(OutstationContext* pContext, const APDUHeader& frag) override final;

	virtual void OnConfirmTimeout(OutstationContext* pContext) override final;

private:

	static OutstationStateSolicitedConfirmWait instance;

	OutstationStateSolicitedConfirmWait() {}
};

/*
class OutstationStateUnsolConfirmWait : public OutstationStateBase, private openpal::Uncopyable
{

public:

	static OutstationStateBase& Inst();

	virtual void OnNewRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality) override final;

	virtual void OnRepeatRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects) override final;

	virtual void OnSendResult(OutstationContext* pContext, bool isSucccess) override final;

	virtual void OnUnsolConfirm(OutstationContext* pContext, const APDUHeader& frag) override final;

	virtual void OnConfirmTimeout(OutstationContext* pContext) override final;

private:

	static OutstationStateUnsolConfirmWait instance;

	OutstationStateUnsolConfirmWait() {}
};
*/




} //ens ns



#endif

