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
#ifndef __MASTER_CONTEXT_H_
#define __MASTER_CONTEXT_H_

#include <openpal/AsyncLayerInterfaces.h>

#include <openpal/IExecutor.h>
#include <openpal/LogRoot.h>
#include <openpal/StaticQueue.h>
#include <openpal/StaticBuffer.h>

#include "opendnp3/master/MasterScheduler.h"

namespace opendnp3
{

class MasterContext : public ICommandProcessor
{
	public:	

	MasterContext(	openpal::IExecutor& executor,
					openpal::LogRoot& root, 
					openpal::ILowerLayer& lower,
					ISOEHandler* pSOEHandler,
					openpal::IUTCTimeSource* pTimeSource,
					const MasterParams& params
				);
	
	openpal::Logger logger;
	openpal::IExecutor* pExecutor;
	openpal::ILowerLayer* pLower;

	// ------- configuration --------
	MasterParams params;
	ISOEHandler* pSOEHandler;

	// ------- dynamic state ---------
	bool isOnline;
	bool isSending;
	uint8_t solSeq;
	uint8_t unsolSeq;
	IMasterTask* pActiveTask;
	openpal::ITimer* pResponseTimer;
	MasterScheduler scheduler;	
	openpal::StaticQueue<APDUHeader, uint8_t, 4> confirmQueue;

	openpal::StaticBuffer<sizes::MAX_TX_APDU_SIZE> txBuffer;
	
	void PostCheckForTask();

	// ------- external events ----------
	bool OnLayerUp();
	bool OnLayerDown();
	void OnSendResult(bool isSucccess);
	void OnReceive(const openpal::ReadOnlyBuffer& apdu);

	// ------- internal events -------

	void OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects);
	void OnUnsolicitedResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects);
	void OnReceiveIIN(const IINField& iin);

	// ------- command events ----------

	virtual void SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback) override final;
	virtual void DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback) override final;

	virtual void SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback) override final;
	virtual void DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback) override final;

	virtual void SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback) override final;
	virtual void DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback) override final;

	virtual void SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback) override final;
	virtual void DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback) override final;

	virtual void SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback) override final;
	virtual void DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback) override final;

	private:	

	void QueueCommandAction(const openpal::Function1<ICommandProcessor*>& action);

	void OnResponseTimeout();

	void CheckForTask();

	void StartTask(IMasterTask* pTask);	

	// -------- helpers --------

	void StartResponseTimer();

	bool CancelResponseTimer();

	void QueueConfirm(const APDUHeader& header);

	bool CheckConfirmTransmit();

	void Transmit(const openpal::ReadOnlyBuffer& output);

	static bool CanConfirmResponse(TaskStatus status);	

	template <class T>
	void SelectAndOperateT(const T& command, uint16_t index, ICommandCallback* pCallback);

	template <class T>
	void DirectOperateT(const T& command, uint16_t index, ICommandCallback* pCallback);
};

template <class T>
void MasterContext::SelectAndOperateT(const T& command, uint16_t index, ICommandCallback* pCallback)
{
	auto process = [command, index, pCallback](ICommandProcessor* pProcessor) {
		pProcessor->SelectAndOperate(command, index, pCallback);
	};
	this->QueueCommandAction(openpal::Bind1<ICommandProcessor*>(process));
}

template <class T>
void MasterContext::DirectOperateT(const T& command, uint16_t index, ICommandCallback* pCallback)
{
	auto process = [command, index, pCallback](ICommandProcessor* pProcessor) {
		pProcessor->DirectOperate(command, index, pCallback);
	};
	this->QueueCommandAction(openpal::Bind1<ICommandProcessor*>(process));
}

}

#endif
