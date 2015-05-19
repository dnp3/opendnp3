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

#include "opendnp3/master/MasterCommandProcessor.h"

namespace opendnp3
{

class MasterContext : public IScheduleCallback
{
	public:	

	MasterContext(	openpal::IExecutor& executor,
					openpal::LogRoot& root, 
					ILowerLayer& lower,
					ISOEHandler& SOEHandler,					
					opendnp3::IMasterApplication& application,
					const MasterParams& params,
					ITaskLock& taskLock
				);	

	// ------- external events ----------
	bool OnLayerUp();
	bool OnLayerDown();
	void OnSendResult(bool isSucccess);
	void OnReceive(const openpal::ReadBufferView& apdu);

	// ------- internal events -------
	
	void OnUnsolicitedResponse(const APDUResponseHeader& response, const openpal::ReadBufferView& objects);
	void OnReceiveIIN(const IINField& iin);	
	

	MasterState mstate;

	ICommandProcessor& GetCommandProcessor()  { return commandProcessor; }

	private:	

	MasterCommandProcessor commandProcessor;
	
	// callback from the scheduler that a task is ready to run	
	virtual void OnPendingTask() override final;				
};

}

#endif
