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
#include "MasterSchedule.h"
#include "Master.h"

#include "AsyncTaskBase.h"
#include "AsyncTaskContinuous.h"
#include "AsyncTaskGroup.h"

#include <functional>

using namespace openpal;

using namespace std;
using namespace std::placeholders;

namespace opendnp3
{

MasterSchedule::MasterSchedule(AsyncTaskGroup* apGroup, Master* apMaster, const MasterConfig& arCfg) :
	mpGroup(apGroup),
	mpMaster(apMaster),
	mTracking(apGroup)	
{
	this->Init(arCfg);
}

void MasterSchedule::EnableOnlineTasks()
{
	mpGroup->Enable(ONLINE_ONLY_TASKS);
}

void MasterSchedule::DisableOnlineTasks()
{
	mpGroup->Disable(ONLINE_ONLY_TASKS);
}

void MasterSchedule::ResetStartupTasks()
{
	mTracking.ResetTasks(START_UP_TASKS);
}

void MasterSchedule::Init(const MasterConfig& arCfg)
{
	mpIntegrityPoll = mTracking.Add(
	                                    arCfg.IntegrityRate,
	                                    arCfg.TaskRetryRate,
	                                    AMP_POLL,
	                                    bind(&Master::IntegrityPoll, mpMaster, _1),
	                                    "Integrity Poll");	

	mpIntegrityPoll->SetFlags(ONLINE_ONLY_TASKS | START_UP_TASKS);

	if (arCfg.DoUnsolOnStartup) {
		/*
		 * DNP3Spec-V2-Part2-ApplicationLayer-_20090315.pdf, page 8
		 * says that UNSOL should be disabled before an integrity scan
		 * is done.
		 */
		TaskHandler handler = bind(&Master::ChangeUnsol,
		                           mpMaster,
		                           _1,
		                           false,
								   CLASS_1 | CLASS_2 | CLASS_3);
		AsyncTaskBase* pUnsolDisable = mTracking.Add(
		                                       TimeDuration::Min(),
		                                       arCfg.TaskRetryRate,
		                                       AMP_UNSOL_CHANGE,
		                                       handler,
		                                       "Unsol Disable");

		pUnsolDisable->SetFlags(ONLINE_ONLY_TASKS | START_UP_TASKS);
		mpIntegrityPoll->AddDependency(pUnsolDisable);

		if (arCfg.EnableUnsol) {
			TaskHandler handler = bind(
			                              &Master::ChangeUnsol,
			                              mpMaster,
			                              _1,
			                              true,
			                              arCfg.UnsolClassMask);

			AsyncTaskBase* pUnsolEnable = mTracking.Add(TimeDuration::Min(),
			                              arCfg.TaskRetryRate,
			                              AMP_UNSOL_CHANGE,
			                              handler,
			                              "Unsol Enable");

			pUnsolEnable->SetFlags(ONLINE_ONLY_TASKS | START_UP_TASKS);
			pUnsolEnable->AddDependency(mpIntegrityPoll);
		}
	}

	/* Tasks are executed when the master is is idle */
	mpCommandTask = mTracking.AddContinuous(
	                        AMP_COMMAND,
	                        std::bind(&Master::ProcessCommand, mpMaster, _1),
	                        "Command");

	mpTimeTask = mTracking.AddContinuous(
	                     AMP_TIME_SYNC,
	                     std::bind(&Master::SyncTime, mpMaster, _1),
	                     "TimeSync");

	mpClearRestartTask = mTracking.AddContinuous(
	                             AMP_CLEAR_RESTART,
	                             std::bind(&Master::WriteIIN, mpMaster, _1),
	                             "Clear IIN");

	mpTimeTask->SetFlags(ONLINE_ONLY_TASKS);
	mpClearRestartTask->SetFlags(ONLINE_ONLY_TASKS);

}

AsyncTaskBase* MasterSchedule::AddClassScan(int aClassMask, TimeDuration aScanRate, TimeDuration aRetryRate)
{
	auto pClassScan = mTracking.Add(		aScanRate,
		                                    aRetryRate,
		                                    AMP_POLL,
											bind(&Master::EventPoll, mpMaster, _1, aClassMask),
		                                    "Class Scan");

	pClassScan->SetFlags(ONLINE_ONLY_TASKS);
	pClassScan->AddDependency(mpIntegrityPoll);
	return pClassScan;
}

}




