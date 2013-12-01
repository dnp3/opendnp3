
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
#ifndef __SLAVE_H_
#define __SLAVE_H_

#include <opendnp3/ICommandHandler.h>
#include <opendnp3/ITimeWriteHandler.h>
#include <opendnp3/SlaveConfig.h>

#include <openpal/Visibility.h>
#include <openpal/ITimer.h>

#include "StackBase.h"
#include "ChangeBuffer.h"
#include "APDU.h"
#include "AppInterfaces.h"
#include "EventBuffers.h"
#include "ObjectReadIterator.h"
#include "ResponseContext.h"
#include "SlaveEventBuffer.h"
#include "SlaveResponseTypes.h"
#include "OutstationSBOHandler.h"


namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

class AS_Base;

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
class DLL_LOCAL Slave : public openpal::Loggable, public IAppUser, public StackBase
{

	friend class AS_Base; //make the state base class a friend
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
	void OnRequest(const APDU&, SequenceInfo);
	void OnUnknownObject();

	/**
	 * Implements IAppUser::IsMaster().
	 *
	 * @return			'false' since this is a Slave (outstation)
	 *					implementation
	 */
	bool IsMaster() {
		return false;
	}

	/**
	 * Returns the buffer that is used for data updates by the user
	 * application.  Writing new entries to this buffer will result in the
	 * Slave doing things.
	 *
	 * @return			a pointer to the buffer
	 */
	IDataObserver* GetDataObserver() {
		return &mChangeBuffer;
	}

private:

	ChangeBuffer mChangeBuffer;				// how client code gives us updates
	IAppLayer* mpAppLayer;					// lower application layer
	Database* mpDatabase;					// holds static data
	ICommandHandler* mpCmdHandler;			// how commands are selected/operated on application code
	int mSequence;							// control sequence
	AS_Base* mpState;						// current state for the state pattern
	SlaveConfig mConfig;					// houses the configurable paramters of the outstation
	SlaveResponseTypes mRspTypes;			// converts the group/var in the config to dnp singletons

	openpal::ITimer* mpUnsolTimer;			// timer for sending unsol responsess
	ITimeWriteHandler* mpTimeWriteHandler;

	INotifier* mpVtoNotifier;

	IINField mIIN;							// IIN bits that persist between requests (i.e. NeedsTime/Restart/Etc)
	IINField mRspIIN;						// Transient IIN bits that get merged before a response is issued
	APDU mResponse;							// APDU used to form responses
	APDU mRequest;							// APDU used to save Deferred requests
	SequenceInfo mSeqInfo;
	APDU mUnsol;							// APDY used to form unsol respones
	ResponseContext mRspContext;			// Used to track and construct response fragments
	OutstationSBOHandler mSBOHandler;       // Tracks SBO requests, forwarding valid sequences to the application

	bool mHaveLastRequest;
	APDU mLastRequest;						// APDU used to form responses


	// Flags that tell us that some action has been Deferred
	// until the slave is in a state capable of handling it.

	bool mDeferredUpdate;					// Indicates that a data update has been Deferred
	bool mDeferredRequest;					// Indicates that a request has been Deferred
	bool mDeferredUnsol;					// Indicates that the unsol timer expired, but the event was Deferred
	bool mDeferredUnknown;

	bool mStartupNullUnsol;					// Tracks whether the device has completed the nullptr unsol startup message

	StackState mState;

	StackState GetState() {
		return mState;
	}

	void UpdateState(StackState aState);

	void OnVtoUpdate();						// internal event dispatched when user code commits an update to mVtoWriter
	void OnDataUpdate();					// internal event dispatched when user code commits an update to mChangeBuffer
	void OnUnsolTimerExpiration();			// internal event dispatched when the unsolicted pack/retry timer expires

	void ConfigureAndSendSimpleResponse();
	void Send(APDU&);
	void Send(APDU& arAPDU, const IINField& arIIN); // overload with additional IIN data
	void SendUnsolicited(APDU& arAPDU);

	void HandleWrite(const APDU& arRequest);
	void HandleWriteIIN(HeaderReadIterator& arHdr);
	void HandleWriteTimeDate(HeaderReadIterator& arHWI);
	void HandleSelect(const APDU& arRequest, SequenceInfo aSeqInfo);
	void HandleOperate(const APDU& arRequest, SequenceInfo aSeqInfo);
	void HandleDirectOperate(const APDU& arRequest, SequenceInfo aSeqInfo);
	void HandleEnableUnsolicited(const APDU& arRequest, bool aIsEnable);
	void HandleUnknown();

	void ConfigureDelayMeasurement(const APDU& arRequest);
	void CreateResponseContext(const APDU& arRequest);

	// Helpers

	size_t FlushUpdates();
	void FlushDeferredEvents();
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
	template <class T>
	void RespondToCommands(const StreamObject<T>* apObj, ObjectReadIterator& arIter, std::function<CommandStatus (T, size_t)> CommandHandler);
};

template<class T>
void Slave::RespondToCommands(const StreamObject<T>* apObj, ObjectReadIterator& arIter, std::function<CommandStatus (T, size_t)> CommandHandler)
{
	IndexedWriteIterator i = mResponse.WriteIndexed(apObj, arIter.Count(), arIter.Header().GetQualifier());
	size_t count = 1;
	while (!arIter.IsEnd()) {
		T val = apObj->Read(*arIter);
		size_t index = arIter->Index();
		if (count > mConfig.mMaxControls) {
			val.mStatus = CS_TOO_MANY_OPS;
		}
		else {
			val.mStatus = CommandHandler(val, index);
			if(val.mStatus == CS_NOT_SUPPORTED) {
				this->mRspIIN.SetParameterError(true);
			}
		}
		i.SetIndex(index);
		apObj->Write(*i, val);
		++i;
		++arIter;
		++count;
	}
}

}

/* vim: set ts=4 sw=4: */

#endif

