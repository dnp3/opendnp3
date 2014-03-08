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
#ifndef __SLAVE_H_
#define __SLAVE_H_

#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/ITimeWriteHandler.h"
#include "opendnp3/outstation/SlaveConfig.h"
#include "opendnp3/outstation/StaticResponseContext.h"
#include "opendnp3/outstation/StaticResponseTypes.h"
#include "opendnp3/outstation/SelectBuffer.h"
#include "opendnp3/outstation/ProxyDataObserver.h"

#include "opendnp3/app/CachedRequest.h"
#include "opendnp3/app/IAppLayer.h"
#include "opendnp3/app/IAppUser.h"

#include "opendnp3/StackBase.h"
#include "opendnp3/StaticSizeConfiguration.h"

#include <openpal/IExecutor.h>
#include <openpal/StaticBuffer.h>

namespace opendnp3
{

class SlaveStateBase;
class Database;

/**
 * @section desc DNP3 outstation.
 *
 * Manages a state machine that handles events from the user layer and the
 * application layer to provide DNP outstation services.
 *
 * ResponseContext and SlaveEventBuffer objects manage data/event responses to
 * master requests, and the IDNPCommandMaster implementation verifies
 * control/setpoint behavior and passes valid commands to the user code.
 *
 * SlaveConfig structure represents the slave behavioral configuration, the
 * Database is in charge of the data model itself.
 *
 * Global IIN state is maintained and combined with request-specific
 * information to form response IINs.
 *
 * The Slave is responsible for building all aspects of APDU packet responses
 * except for the application sequence number.
 */
class Slave : public IAppUser, public StackBase
{
	// states can operate back on the slave's private functions
	friend class SlaveStateBase;
	friend class AS_OpenBase;
	friend class AS_Closed;
	friend class AS_Idle;
	friend class AS_WaitForRspSuccess;
	friend class AS_WaitForUnsolSuccess;
	friend class AS_WaitForSolUnsolSuccess;	

public:

	Slave(openpal::Logger, IAppLayer*, openpal::IExecutor*, ITimeWriteHandler* apWriteHandler, Database*, ICommandHandler*, const SlaveConfig&);
	~Slave();

	////////////////////////
	// External events
	////////////////////////

	void SetNeedTimeIIN();

	/* Implement IAppUser - callbacks from the app layer */
	void OnLowerLayerUp();
	void OnLowerLayerDown();

	void OnUnsolSendSuccess();
	void OnSolSendSuccess();

	void OnUnsolFailure();
	void OnSolFailure();

	// Only have to override OnRequest since we're a slave
	void OnRequest(const APDURecord&, SequenceInfo);
	
	/**
	 * Returns the buffer that is used for data updates by the user
	 * application.  Writing new entries to this buffer will result in the
	 * Slave doing things.
	 *
	 * @return			a pointer to the buffer
	 */
	IDataObserver* GetDataObserver() 
	{
		return &proxyObserver;
	}

private:

	void ChangeState(SlaveStateBase* apState);

	void RespondToRequest(const APDURecord& record, SequenceInfo sequence);
	IINField ConfigureResponse(const APDURecord& request, SequenceInfo sequence, APDUResponse& response);	

	openpal::StaticBuffer<SizeConfiguration::MAX_APDU_BUFFER_SIZE> responseBuffer;
	SelectBuffer selectBuffer;

	APDUResponse lastResponse;				// wrapper that points to the last response made

	ProxyDataObserver proxyObserver;		// how client code gives us updates
	IAppLayer* mpAppLayer;					// lower application layer
	Database* mpDatabase;					// holds static data
	ICommandHandler* mpCmdHandler;			// how commands are selected/operated on application code
	int mSequence;							// control sequence
	SlaveStateBase* mpState;				// current state for the state pattern
	SlaveConfig mConfig;					// houses the configurable paramters of the outstation	

	openpal::ITimer* mpUnsolTimer;			// timer for sending unsol responsess
	ITimeWriteHandler* mpTimeWriteHandler;	

	IINField mIIN;							// IIN bits that persist between requests (i.e. NeedsTime/Restart/Etc)		
	CachedRequest mCachedRequest;			// Request cache for when outstation needs to defer a request
		
	StaticResponseContext mStaticRspContext;	// Used to track and construct static response fragments	


	// Flags that tell us that some action has been Deferred
	// until the slave is in a state capable of handling it.
	
	bool mDeferredUnsol;					// Indicates that the unsol timer expired, but the event was Deferred	
	bool mStartupNullUnsol;					// Tracks whether the device has completed the nullptr unsol startup message

	StackState mState;

	StackState GetState() {
		return mState;
	}

	void UpdateState(StackState aState);
	
	void OnDataUpdate();					// internal event dispatched when user code commits an update to mChangeBuffer
	void OnUnsolTimerExpiration();			// internal event dispatched when the unsolicted pack/retry timer expires
	
	void SendResponse(APDUResponse& apdu, const IINField& indications = IINField::Empty);
	//void SendUnsolicited(APDU& apdu, const IINField& indications = IINField::Empty);

	IINField HandleWrite(const APDURecord& request, SequenceInfo sequence);
	IINField HandleRead(const APDURecord& request, SequenceInfo sequence, APDUResponse& response);
	IINField HandleSelect(const APDURecord& request, SequenceInfo sequence, APDUResponse& response);
	IINField HandleOperate(const APDURecord& request, SequenceInfo sequence, APDUResponse& response);
	IINField HandleDirectOperate(const APDURecord& request, SequenceInfo sequence, APDUResponse& response);
	IINField HandleDelayMeasure(const APDURecord& request, SequenceInfo sequence, APDUResponse& response);


	IINField HandleCommandWithConstant(const APDURecord& request, APDUResponse& response, CommandStatus status);

	void ContinueResponse();	

	// Helpers
	
	void StartUnsolTimer(openpal::TimeDuration aTimeout);

	// Task handlers

	void ResetTimeIIN();
	openpal::ITimer* mpTimeTimer;

	/**
	 * A structure to provide the C++ equivalent of templated typedefs.
	 */
	template <class T>
	struct CommandFunc {
		typedef std::function<CommandStatus (T&, size_t)> Type;
	};

	/**
	 * Forms a response message to a list of command objects.
	 * Slave::mResponse is used as a destination buffer.
	 *
	 * @param apObj			DNP3 object capable of reading/writing its own
	 * 						type from/to a byte stream
	 * @param arIter		an ObjectReadIterator that provides access to the
	 * 						DNP3 objects
	 * @param aFunc			Function for issuing/selecting
	 */
	//template <class T>
	//void RespondToCommands(const StreamObject<T>* apObj, ObjectReadIterator& arIter, std::function<CommandStatus (T, size_t)> CommandHandler);
};

/*
template<class T>
void Slave::RespondToCommands(const StreamObject<T>* apObj, ObjectReadIterator& arIter, std::function<CommandStatus (T, size_t)> CommandHandler)
{
	IndexedWriteIterator i = mResponse.WriteIndexed(apObj, arIter.Count(), arIter.Header().GetQualifier());
	size_t count = 1;
	while (!arIter.IsEnd()) {
		T val = apObj->Read(*arIter);
		size_t index = arIter->Index();
		if (count > mConfig.mMaxControls) {
			val.status = CommandStatus::TOO_MANY_OPS;
		}
		else {
			val.status = CommandHandler(val, index);
			if(val.status == CommandStatus::NOT_SUPPORTED) {
				this->mRspIIN.Set(IINBit::PARAM_ERROR);
			}
		}
		i.SetIndex(index);
		apObj->Write(*i, val);
		++i;
		++arIter;
		++count;
	}
}
*/

}



#endif

