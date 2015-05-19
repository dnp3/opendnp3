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
#include "opendnp3/master/MasterCommandProcessor.h"
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

	virtual void OnReceive(const openpal::ReadBufferView&) override final;
	
	virtual void OnSendResult(bool isSucccess) override final;

	/// ----- Misc public members -------
	
	ICommandProcessor& GetCommandProcessor();

	/// ---- Permanently bound scans ----

	MasterScan AddScan(openpal::TimeDuration period, const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback = nullptr, int userId = -1);

	MasterScan AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, ITaskCallback* pCallback = nullptr, int userId = -1);

	MasterScan AddClassScan(const ClassField& field, openpal::TimeDuration period, ITaskCallback* pCallback = nullptr, int userId = -1);

	MasterScan AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, ITaskCallback* pCallback = nullptr, int userId = -1);

	/// ---- Single shot immediate scans ----
	
	void Scan(const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback = nullptr, int userId = -1);

	void ScanAllObjects(GroupVariationID gvId, ITaskCallback* pCallback = nullptr, int userId = -1);

	void ScanClasses(const ClassField& field, ITaskCallback* pCallback = nullptr, int userId = -1);

	void ScanRange(GroupVariationID gvId, uint16_t start, uint16_t stop, ITaskCallback* pCallback = nullptr, int userId = -1);

	/// ---- Write tasks -----

	void Write(const TimeAndInterval& value, uint16_t index, ITaskCallback* pCallback = nullptr, int userId = -1);

	
	private:

	void ScheduleRecurringPollTask(IMasterTask* pTask);
	void ScheduleAdhocTask(IMasterTask* pTask);

	MasterContext context;
	MasterCommandProcessor commandProcessor;
};

}

#endif
