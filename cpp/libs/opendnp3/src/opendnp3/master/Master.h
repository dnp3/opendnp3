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
#ifndef __MASTER_H_
#define __MASTER_H_

#include "opendnp3/master/MasterContext.h"
#include "opendnp3/master/CommandMarshaller.h"
#include "opendnp3/master/MasterScan.h"
#include "opendnp3/master/IMasterApplication.h"

namespace opendnp3
{

class Master : public IUpperLayer
{
	public:

	Master(	openpal::IExecutor& executor, 				
			openpal::LogRoot& root, 
			ILowerLayer& lower,
			ISOEHandler& SOEHandler,			
			opendnp3::IMasterApplication& application,
			const MasterParams& params,
			ITaskLock& taskLock = NullTaskLock::Instance()
			);
	
	/// ----- Implement IUpperLayer ------

	virtual void OnLowerLayerUp() override final;
	
	virtual void OnLowerLayerDown() override final;

	virtual void OnReceive(const openpal::ReadOnlyBuffer&) override final;
	
	virtual void OnSendResult(bool isSucccess) override final;

	/// ----- Misc public members -------
	
	ICommandProcessor& GetCommandProcessor();

	MasterScan AddScan(openpal::TimeDuration period, const openpal::Action1<APDURequest&> builder);

	MasterScan AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period);

	MasterScan AddClassScan(const ClassField& field, openpal::TimeDuration period);	

	MasterScan AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period);
	
	private:

	MasterContext context;
	CommandMarshaller commandMarshaller;
};

}

#endif
