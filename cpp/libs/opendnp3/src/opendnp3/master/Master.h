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
#ifndef OPENDNP3_MASTER_H
#define OPENDNP3_MASTER_H

#include "opendnp3/app/HeaderWriter.h"

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

	/// ---- Permanently bound scans ----

	MasterScan AddScan(openpal::TimeDuration period, const std::function<void(HeaderWriter&)>& builder, int id, ITaskCallback* pCallback);

	MasterScan AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, int id, ITaskCallback* pCallback);

	MasterScan AddClassScan(const ClassField& field, openpal::TimeDuration period, int id, ITaskCallback* pCallback);

	MasterScan AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, int id, ITaskCallback* pCallback);

	/// ---- Single shot immediate scans ----
	
	void Scan(const std::function<void(HeaderWriter&)>& builder, int id, ITaskCallback* pCallback);

	void ScanAllObjects(GroupVariationID gvId, int id, ITaskCallback* pCallback);

	void ScanClasses(const ClassField& field, int id, ITaskCallback* pCallback);

	void ScanRange(GroupVariationID gvId, uint16_t start, uint16_t stop, int id, ITaskCallback* pCallback);

	/// ---- Write tasks -----

	void Write(const TimeAndInterval& value, uint16_t index, int id, ITaskCallback* pCallback);

	
	private:

	MasterContext context;
	CommandMarshaller commandMarshaller;
};

}

#endif
