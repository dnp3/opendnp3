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
#ifndef __SLAVE_STATES_H_
#define __SLAVE_STATES_H_

#include <string>

#include "opendnp3/Singleton.h"

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/SequenceInfo.h"

namespace opendnp3
{

class Outstation;

/**
 * Base class for all transaction states of a Slave
 */
class SlaveStateBase
{
public:

	virtual std::string Name() const = 0;

	/* Events from application layer */

	virtual void OnLowerLayerUp(Outstation*);
	virtual void OnLowerLayerDown(Outstation*);

	virtual void OnSolSendSuccess(Outstation*);
	virtual void OnSolFailure(Outstation*);

	virtual void OnUnsolSendSuccess(Outstation*);
	virtual void OnUnsolFailure(Outstation*);

	virtual void OnRequest(Outstation*, const APDURecord&, SequenceInfo);

	virtual void Enter(Outstation*) = 0;

	// Called when a data update is received from the user layer
	virtual void OnDataUpdate(Outstation* slave) {}

	// Called when the unsolcited timer expires
	virtual void OnUnsolExpiration(Outstation*);


protected:

	void DoUnsolSuccess(Outstation*);
};

/**
 * A state that indicates when the application layer has not informed the
 * slave yet that it is up.
 */
class AS_Closed : public SlaveStateBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_Closed);

	void Enter(Outstation*) final;

	void OnLowerLayerUp(Outstation* slave) final;
	void OnDataUpdate(Outstation*) final;
};

class AS_OpenBase : public SlaveStateBase
{
public:

	void OnLowerLayerDown(Outstation*) final;

};

/**
 * A state that indicates when the AppLayer is online but the Slave is not
 * doing or waiting on anything.
 */
class AS_Idle : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_Idle);

	void Enter(Outstation*) final;
	void OnDataUpdate(Outstation*) final;
	void OnUnsolExpiration(Outstation*) final;
	void OnRequest(Outstation*, const APDURecord&, SequenceInfo) final;

};


/**
 * A state that indicates when the Slave is waiting for a response to
 * complete.
 */
class AS_WaitForRspSuccess : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_WaitForRspSuccess);

	void Enter(Outstation*) final {}
	void OnRequest(Outstation*, const APDURecord&, SequenceInfo) final;
	void OnSolFailure(Outstation*) final;
	void OnSolSendSuccess(Outstation*) final;

};

/**
 * A state that indicates when the Slave is waiting for an unsolicited
 * response to complete.
 */
class AS_WaitForUnsolSuccess : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_WaitForUnsolSuccess);

	void Enter(Outstation*) final {}
	void OnRequest(Outstation*, const APDURecord&, SequenceInfo) final;
	void OnUnsolFailure(Outstation*) final;
	void OnUnsolSendSuccess(Outstation*) final;

};

/**
 * A state that indicates when the Slave is waiting for an unsolicited
 * response and a solicited response to complete.
 */
class AS_WaitForSolUnsolSuccess : public AS_OpenBase
{
public:

	MACRO_STATE_SINGLETON_INSTANCE(AS_WaitForSolUnsolSuccess);

	void Enter(Outstation*) final {}
	void OnRequest(Outstation*, const APDURecord&, SequenceInfo) final;
	void OnSolFailure(Outstation*) final;
	void OnSolSendSuccess(Outstation*) final;
	void OnUnsolFailure(Outstation*) final;
	void OnUnsolSendSuccess(Outstation*) final;

};

} //ens ns



#endif

