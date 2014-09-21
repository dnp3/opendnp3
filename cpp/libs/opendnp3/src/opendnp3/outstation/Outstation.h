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
#ifndef OPENDNP3_OUTSTATION_H
#define OPENDNP3_OUTSTATION_H

#include "opendnp3/LayerInterfaces.h"

#include "opendnp3/outstation/OutstationContext.h"

namespace opendnp3
{

class Outstation : public IUpperLayer
{
	public:

	Outstation(	const OutstationConfig& config,
					openpal::IExecutor& executor, 
					openpal::LogRoot& root, 
					ILowerLayer& lower,
					ICommandHandler& commandHandler,
					IOutstationApplication& application,
					Database& database, 
					const EventBufferFacade& buffers);
	
	/// ----- Implement IUpperLayer ------

	virtual void OnLowerLayerUp() override final;
	
	virtual void OnLowerLayerDown() override final;

	virtual void OnReceive(const openpal::ReadOnlyBuffer&) override final;
	
	virtual void OnSendResult(bool isSucccess) override final;

	/// ---- Other public members

	void SetRestartIIN();
	
	
	private:

	// ------ Internal events ------			

	OutstationContext context;

};

}

#endif
