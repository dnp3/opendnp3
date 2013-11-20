
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
#ifndef __MASTER_H_
#define __MASTER_H_

#include <opendnp3/MasterConfig.h>
#include <opendnp3/ObjectInterfaces.h>
#include <opendnp3/IMeasurementHandler.h>

#include <openpal/Visibility.h>
#include <openpal/IExecutor.h>
#include <openpal/IUTCTimeSource.h>

#include "APDU.h"
#include "AppInterfaces.h"
#include "ObjectReadIterator.h"
#include "MasterSchedule.h"
#include "QueuedCommandProcessor.h"
#include "CommandHelpers.h"

// includes for tasks
#include "StartupTasks.h"
#include "DataPoll.h"
#include "CommandTask.h"
#include "StackBase.h"

namespace openpal
{

class IExecutor;

}

namespace opendnp3
{

class IDataObserver;
class ITask;
class AsyncTaskGroup;
class ITimeSource;
class AsyncTaskContinuous;
class AsyncTaskBase;
class CopyableBuffer;
class AMS_Base;

/**
 * Represents a DNP3 Master endpoint. The tasks functions can perform all the
 * various things that a Master might need to do.
 *
 * Coordination of tasks is handled by a higher level task scheduler.
 */
class DLL_LOCAL Master : public openpal::Loggable, public IAppUser, public StackBase, private ICommandProcessor
{
	friend class AMS_Base;
	friend class AMS_Idle;
	friend class AMS_OpenBase;
	friend class AMS_Waiting;
	friend class MasterSchedule;

public:

	Master(openpal::Logger, MasterConfig aCfg, IAppLayer*, IMeasurementHandler*, AsyncTaskGroup*, openpal::IExecutor*, openpal::IUTCTimeSource* apTimeSrc);
	virtual ~Master() {}

	ICommandProcessor* GetCommandProcessor() {
		return &mCommandQueue;
	}

	/* Implement IAppUser - callbacks from the app layer */

	void OnLowerLayerUp();
	void OnLowerLayerDown();

	void OnSolSendSuccess();
	void OnSolFailure();

	void OnUnsolSendSuccess();
	void OnUnsolFailure();

	// override the response functions
	void OnPartialResponse(const APDU&);
	void OnFinalResponse(const APDU&);
	void OnUnsolResponse(const APDU&);

	/**
	 * Implements IAppUser::IsMaster().
	 *
	 * @return			'true' since this is a Master implementation
	 */
	bool IsMaster() {
		return true;
	}

	// These methods are inherited privately
	void SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

	void SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);
	void DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback);

private:

	template <class T>
	void ConfigureCommandTask(const T& arCommand, size_t aIndex, CommandObject<T>* apObj, std::function<void (CommandResponse)> aCallback) {
		auto formatter = [ = ](APDU & arAPDU, FunctionCodes aCode) {
			return CommandHelpers::ConfigureRequest(arAPDU, aCode, arCommand, aIndex, apObj);
		};
		auto responder = [ = ](CommandStatus aStatus) {
			mpExecutor->Post([ = ]() {
				aCallback(CommandResponse(aStatus));
			});
		};
		mCommandTask.Configure(formatter, responder);
	}

	void UpdateState(StackState aState);

	/* Task functions used for scheduling */

	void WriteIIN(ITask* apTask);
	void IntegrityPoll(ITask* apTask);
	void EventPoll(ITask* apTask, int aClassMask);
	void ChangeUnsol(ITask* apTask, bool aEnable, int aClassMask);
	void SyncTime(ITask* apTask);
	void ProcessCommand(ITask* apTask);
	//void TransmitVtoData(ITask* apTask);

	IINField mLastIIN;						// last IIN received from the outstation

	void ProcessIIN(const IINField& arIIN);	// Analyze IIN bits and react accordingly
	void ProcessDataResponse(const APDU&);	// Read data output of solicited or unsolicited response and publish
	void StartTask(MasterTaskBase*, bool aInit);	// Starts a task running

	QueuedCommandProcessor mCommandQueue;				// Threadsafe queue for buffering command requests

	APDU mRequest;							// APDU that gets reused for requests

	IAppLayer* mpAppLayer;					// lower application layer
	IMeasurementHandler* mpPublisher;	    // where the measurements are pushed
	AsyncTaskGroup* mpTaskGroup;			// How task execution is controlled
	openpal::IUTCTimeSource* mpTimeSrc;		// Access to UTC, normally system time but can be a mock for testing

	AMS_Base* mpState;						// Pointer to active state, start in TLS_Closed
	MasterTaskBase* mpTask;					// The current master task
	ITask* mpScheduledTask;					// The current scheduled task
	StackState mState;						// Current state of the master

	StackState GetState() {
		return mState;
	}

	/* --- Task plumbing --- */

	MasterSchedule mSchedule;				// The machinery needed for scheduling

	ClassPoll mClassPoll;					// used to perform integrity/exception scans
	ClearRestartIIN mClearRestart;			// used to clear the restart
	ConfigureUnsol mConfigureUnsol;			// manipulates how the outstation does unsolictied reporting
	TimeSync mTimeSync;						// performs time sync on the outstation
	CommandTask mCommandTask;				// performs command execution

};

}

/* vim: set ts=4 sw=4: */

#endif
