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
#ifndef OPENDNP3_MASTERCONTEXT_H
#define OPENDNP3_MASTERCONTEXT_H

#include <openpal/executor/IExecutor.h>
#include <openpal/logging/LogRoot.h>
#include <openpal/container/Buffer.h>
#include <openpal/executor/TimerRef.h>

#include "opendnp3/LayerInterfaces.h"

#include "opendnp3/app/AppSeqNum.h"
#include "opendnp3/master/MasterScheduler.h"
#include "opendnp3/master/MasterTasks.h"
#include "opendnp3/master/ITaskLock.h"
#include "opendnp3/master/IMasterApplication.h"

#include <deque>

namespace opendnp3
{
	/*
		All of the mutable state and configuration for a master
	*/
	class MContext : private IScheduleCallback, private openpal::Uncopyable
	{

	protected:

		enum class TaskState
		{
			IDLE,
			TASK_READY,
			WAIT_FOR_RESPONSE
		};

	public:

		MContext(
			openpal::IExecutor& executor,
			openpal::LogRoot& root,
			ILowerLayer& lower,
			ISOEHandler& SOEHandler,
			opendnp3::IMasterApplication& application,			
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


		// ------- dynamic state ---------
		bool isOnline;
		bool isSending;
		AppSeqNum solSeq;
		AppSeqNum unsolSeq;
		openpal::ManagedPtr<IMasterTask> pActiveTask;		
		openpal::TimerRef responseTimer;
		openpal::TimerRef scheduleTimer;
		MasterTasks tasks;
		MasterScheduler scheduler;
		std::deque<APDUHeader> confirmQueue;
		openpal::Buffer txBuffer;
		TaskState tstate;
	
		void OnReceive(const openpal::ReadBufferView& apdu);

		/// virtual methods that can be overriden to implement secure authentication		

		virtual bool GoOnline();

		virtual bool GoOffline();
		
		virtual void OnParsedHeader(const openpal::ReadBufferView& apdu, const APDUResponseHeader& header, const openpal::ReadBufferView& objects);

		virtual void RecordLastRequest(const openpal::ReadBufferView& apdu) {}

		/// public state manipulation actions

		bool BeginNewTask(openpal::ManagedPtr<IMasterTask>& task);

		bool ResumeActiveTask();

		void CompleteActiveTask();

		void QueueConfirm(const APDUHeader& header);

		void StartResponseTimer();		

		void ProcessAPDU(const APDUResponseHeader& header, const openpal::ReadBufferView& objects);

		void CheckForTask();

		bool CheckConfirmTransmit();

		void PostCheckForTask();

		void ProcessResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects);

		void ProcessUnsolicitedResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects);

		void Transmit(const openpal::ReadBufferView& data);

	private:	

		virtual void OnPendingTask() override { this->PostCheckForTask(); }

		void ProcessIIN(const IINField& iin);		
							
		void OnResponseTimeout();		

	protected:
				
		/// state switch lookups
		TaskState OnStartEvent();
		TaskState OnResponseEvent(const APDUResponseHeader& header, const openpal::ReadBufferView& objects);
		TaskState OnResponseTimeoutEvent();

		/// --- state handling functions ----

		TaskState StartTask_Idle();
		TaskState StartTask_TaskReady();

		TaskState OnResponse_WaitForResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects);
		TaskState OnResponseTimeout_WaitForResponse();
	};

}

#endif
