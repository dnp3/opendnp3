//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
// contributor license agreements. See the NOTICE file distributed with this
// work for additional information regarding copyright ownership.  Green Enery
// Corp licenses this file to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
// License for the specific language governing permissions and limitations
// under the License.
//

#ifndef __SLAVE_H_
#define __SLAVE_H_

#

#include <APL/CachedLogVariable.h>
#include <APL/ChangeBuffer.h>
#include <APL/CommandResponseQueue.h>
#include <APL/Loggable.h>
#include <APL/Logger.h>
#include <APL/PostingNotifierSource.h>
#include <APL/TimeSource.h>
#include <APL/LoggableMacros.h>

#include "APDU.h"
#include "AppInterfaces.h"
#include "DNPCommandMaster.h"
#include "EventBuffers.h"
#include "ObjectReadIterator.h"
#include "ResponseContext.h"
#include "SlaveConfig.h"
#include "SlaveEventBuffer.h"
#include "SlaveResponseTypes.h"
#include "VtoReader.h"
#include "VtoWriter.h"
#include "IStackObserver.h"

namespace apl
{
class IExecutor;
}

namespace apl
{
namespace dnp
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
class Slave : public Loggable, public IAppUser
{

	friend class AS_Base; //make the state base class a friend
	friend class AS_OpenBase;
	friend class AS_Closed;
	friend class AS_Idle;
	friend class AS_WaitForRspSuccess;
	friend class AS_WaitForUnsolSuccess;
	friend class AS_WaitForSolUnsolSuccess;

public:

	Slave(Logger*, IAppLayer*, IExecutor*, ITimeManager*, Database*, IDNPCommandMaster*, const SlaveConfig&);
	~Slave();

	////////////////////////
	// External events
	////////////////////////

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

	/**
	 * Returns a pointer to the VTO reader object.  This should only be
	 * used by internal subsystems in the library.  External user
	 * applications should associate IVtoCallbacks objects using the
	 * AsyncStackManager.
	 *
	 * @return			a pointer to the VtoReader instance for this stack
	 */
	VtoReader* GetVtoReader() {
		return &mVtoReader;
	}

	/**
	 * Returns a pointer to the VtoWriter instance for this stack.
	 * External user applications should use this hook to write new data
	 * to the Master via the Slave (outstation).
	 *
	 * @return			a pointer to the VtoWriter instance for this stack
	 */
	IVtoWriter* GetVtoWriter() {
		return &mVtoWriter;
	}

private:

	ChangeBuffer mChangeBuffer;				// how client code gives us updates
	PostingNotifierSource mNotifierSource;	// way to get special notifiers for the change queue / vto
	IAppLayer* mpAppLayer;					// lower application layer
	IExecutor* mpExecutor;				// used for post and timers
	Database* mpDatabase;					// holds static data
	IDNPCommandMaster* mpCmdMaster;			// how commands are selected/operated
	int mSequence;							// control sequence
	CommandResponseQueue mRspQueue;			// how command responses are received
	AS_Base* mpState;						// current state for the state pattern
	SlaveConfig mConfig;					// houses the configurable paramters of the outstation
	SlaveResponseTypes mRspTypes;			// converts the group/var in the config to dnp singletons

	ITimer* mpUnsolTimer;					// timer for sending unsol responsess

	INotifier* mpVtoNotifier;

	IINField mIIN;							// IIN bits that persist between requests (i.e. NeedsTime/Restart/Etc)
	IINField mRspIIN;						// Transient IIN bits that get merged before a response is issued
	APDU mResponse;							// APDU used to form responses
	APDU mRequest;							// APDU used to save Deferred requests
	SequenceInfo mSeqInfo;
	APDU mUnsol;							// APDY used to form unsol respones
	ResponseContext mRspContext;			// Used to track and construct response fragments

	bool mHaveLastRequest;
	APDU mLastRequest;						// APDU used to form responses

	ITimeManager* mpTime;
	CachedLogVariable mCommsStatus;

	// Flags that tell us that some action has been Deferred
	// until the slave is in a state capable of handling it.

	bool mDeferredUpdate;					// Indicates that a data update has been Deferred
	bool mDeferredRequest;					// Indicates that a request has been Deferred
	bool mDeferredUnsol;					// Indicates that the unsol timer expired, but the event was Deferred
	bool mDeferredUnknown;

	bool mStartupNullUnsol;					// Tracks whether the device has completed the NULL unsol startup message

	IStackObserver* mpObserver;             // update consumers who want to know when dnp3 connection state changes
	StackStates mState;

	void UpdateState(StackStates aState);

	void OnVtoUpdate();						// internal event dispatched when user code commits an update to mVtoWriter
	void OnDataUpdate();					// internal event dispatched when user code commits an update to mChangeBuffer
	void OnUnsolTimerExpiration();			// internal event dispatched when the unsolicted pack/retry timer expires

	void ConfigureAndSendSimpleResponse();
	void Send(APDU&);
	void Send(APDU& arAPDU, const IINField& arIIN); // overload with additional IIN data
	void SendUnsolicited(APDU& arAPDU);

	void HandleWrite(const APDU& arRequest);
	void HandleVtoTransfer(const APDU& arRequest);
	void HandleWriteIIN(HeaderReadIterator& arHdr);
	void HandleWriteTimeDate(HeaderReadIterator& arHWI);
	void HandleWriteVto(HeaderReadIterator& arHdr);
	void HandleSelect(const APDU& arRequest, SequenceInfo aSeqInfo);
	void HandleOperate(const APDU& arRequest, SequenceInfo aSeqInfo);
	void HandleDirectOperate(const APDU& arRequest, SequenceInfo aSeqInfo);
	void HandleEnableUnsolicited(const APDU& arRequest, bool aIsEnable);
	void HandleUnknown();

	void ConfigureDelayMeasurement(const APDU& arRequest);
	void CreateResponseContext(const APDU& arRequest);

	// Helpers

	size_t FlushVtoUpdates();
	size_t FlushUpdates();
	void FlushDeferredEvents();
	void StartUnsolTimer(millis_t aTimeout);

	// Task handlers

	void ResetTimeIIN();
	ITimer* mpTimeTimer;

	/**
	 * The VtoReader instance for this stack which will direct received
	 * VTO data to the user application.  The user application should
	 * register an IVtoCallbacks instance for the desired virtual channel
	 * id(s) using AsyncStackManager::AddVtoChannel().
	 */
	VtoReader mVtoReader;

	/**
	 * The VtoWriter instance for this stack which will buffer new data
	 * from the user application to the DNP3 stream.  This handler is
	 * thread-safe.
	 */
	VtoWriter mVtoWriter;

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
	void RespondToCommands(const StreamObject<T>* apObj, ObjectReadIterator& arIter, typename CommandFunc<T>::Type aFunc);

	/**
	 * Marks a number of command responses from Slave::mpCmdMaster as
	 * selected.
	 *
	 * @param arCmd			an ObjectBase instance that is capable of
	 * 						reading/writing its own type from/to a byte stream
	 * @param aIndex		the maximum number of command responses to select
	 * @param aHdr			the DNP3 object header in the APDU message
	 * @param aSeqInfo		descriptive information about the sequence number
	 * @param aSeqNum		the sequence number from the APDU message
	 *
	 * @return				the appropriate CommandStatus value based on the
	 * 						results of the function
	 */
	template <class T>
	CommandStatus Select(T aCmd, size_t aIndex, HeaderInfo aHdr, SequenceInfo aSeqInfo, int aSeqNum);

	/**
	 * TODO - I have no clue what this function does.
	 *
	 * @param arCmd			an ObjectBase instance that is capable of
	 * 						reading/writing its own type from/to a byte stream
	 * @param aIndex		the maximum number of command responses to select
	 * @param aDirect		a flag to indicate whether the execution should use
	 * 						mpCmdMaster's Operate() or DirectOperate()
	 * @param aHdr			the DNP3 object header in the APDU message
	 * @param aSeqInfo		descriptive information about the sequence number
	 * @param aSeqNum		the sequence number from the APDU message
	 *
	 * @return				the appropriate CommandStatus value based on the
	 * 						results of the function
	 */
	template <class T>
	CommandStatus Operate(T& arCmd, size_t aIndex, bool aDirect, const HeaderInfo& aHdr, SequenceInfo aSeqInfo, int aSeqNum);

};

template<class T>
void Slave::RespondToCommands(const StreamObject<T>* apObj, ObjectReadIterator& arIter, typename CommandFunc<T>::Type arFunc)
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
			val.mStatus = arFunc(val, index);
		}
		i.SetIndex(index);
		apObj->Write(*i, val);
		++i;
		++arIter;
		++count;
	}
}

template <class T>
CommandStatus Slave::Select(T aCmd, size_t aIndex, HeaderInfo aHdr, SequenceInfo aSeqInfo, int aSeqNum)
{
	CommandStatus res = mpCmdMaster->Select(CommandRequestInfo<T>(aCmd, aHdr.GetObjectType(), aHdr.GetVariation(), aHdr.GetQualifier(), aSeqInfo, aSeqNum), aIndex) ? CS_SUCCESS : CS_NOT_SUPPORTED;
	LOG_BLOCK(LEV_INFO, "Selecting " << aCmd.ToString() << " Index: " << aIndex << " Result: " << ToString(res));
	if (res == CS_NOT_SUPPORTED) {
		mRspIIN.SetParameterError(true);
	}
	if (res == CS_TOO_MANY_OPS) {
		mpCmdMaster->DeselectAll(); // 4.4.3 rule 3
	}
	return res;
}

template <class T>
CommandStatus Slave::Operate(T& arCmd, size_t aIndex, bool aDirect, const HeaderInfo& aHdr, SequenceInfo aSeqInfo, int aSeqNum)
{
	++mSequence;
	CommandStatus res;
	if (aDirect) {
		res = mpCmdMaster->DirectOperate(CommandRequestInfo<T>(arCmd, aHdr.GetObjectType(), aHdr.GetVariation(), aHdr.GetQualifier(), aSeqInfo, aSeqNum), aIndex, mSequence);
	}
	else {
		res = mpCmdMaster->Operate(CommandRequestInfo<T>(arCmd, aHdr.GetObjectType(), aHdr.GetVariation(), aHdr.GetQualifier(), aSeqInfo, aSeqNum), aIndex, mSequence);
	}

	if (res != CS_SUCCESS) {
		if (res == CS_NOT_SUPPORTED) {
			mRspIIN.SetParameterError(true);
		}
		return res;
	}
	else {
		CommandResponse cr(CS_HARDWARE_ERROR);
		mRspQueue.WaitForResponse(cr, mSequence); // wait forever on a response from user space
		LOG_BLOCK(LEV_INFO, arCmd.ToString() << " Index: " << aIndex << " Result: " << ToString(cr.mResult));
		return cr.mResult;
	}
}

}
}

/* vim: set ts=4 sw=4: */

#endif
