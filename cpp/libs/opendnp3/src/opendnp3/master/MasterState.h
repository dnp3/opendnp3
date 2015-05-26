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
#ifndef OPENDNP3_MASTERSTATE_H
#define OPENDNP3_MASTERSTATE_H

#include <openpal/executor/IExecutor.h>
#include <openpal/logging/LogRoot.h>
#include <openpal/container/Buffer.h>
#include <openpal/executor/TimerRef.h>

#include "opendnp3/LayerInterfaces.h"

#include "opendnp3/app/AppSeqNum.h"
#include "opendnp3/master/MasterScheduler.h"
#include "opendnp3/master/MasterTasks.h"
#include "opendnp3/master/IMasterState.h"
#include "opendnp3/master/ITaskLock.h"
#include "opendnp3/master/IMasterApplication.h"
#include "opendnp3/master/MasterAuthWrapper.h"

#include <deque>

namespace opendnp3
{
	/*
		All of the mutable state and configuration for a master
	*/
	class MasterState
	{

	public:

		MasterState(openpal::IExecutor& executor,
			openpal::LogRoot& root,
			ILowerLayer& lower,
			ISOEHandler& SOEHandler,
			opendnp3::IMasterApplication& application,
			IScheduleCallback& scheduleCallback,
			const MasterParams& params,
			ITaskLock& taskLock
			);	

		openpal::Logger logger;
		openpal::IExecutor* pExecutor;
		ILowerLayer* pLower;

		// ------- configuration --------
		MasterParams params;
		ISOEHandler* pSOEHandler;
		ITaskLock* pTaskLock;
		IMasterApplication* pApplication;
		IScheduleCallback* pScheduleCallback;
		MasterAuthWrapper auth;


		// ------- dynamic state ---------
		bool isOnline;
		bool isSending;
		AppSeqNum solSeq;
		AppSeqNum unsolSeq;
		openpal::ManagedPtr<IMasterTask> pActiveTask;		
		openpal::TimerRef responseTimer;
		MasterTasks tasks;
		MasterScheduler scheduler;
		std::deque<APDUHeader> confirmQueue;
		openpal::Buffer txBuffer;
		IMasterState* pState;	


	public:

		/// public state manipulation actions

		bool GoOnline();

		bool GoOffline();		

		bool BeginNewTask(openpal::ManagedPtr<IMasterTask>& task);

		bool ResumeActiveTask();

		void CompleteActiveTask();

		void QueueConfirm(const APDUHeader& header);

		void StartResponseTimer();
				
		void OnResponse(const APDUResponseHeader& response, const openpal::ReadBufferView& objects);

		void CheckForTask();

		bool CheckConfirmTransmit();

		void PostCheckForTask();

	private:	

		/// private state manipulation actions used from the public actions
							
		void OnResponseTimeout();		

		void Transmit(const openpal::ReadBufferView& data);
	};

}

#endif
