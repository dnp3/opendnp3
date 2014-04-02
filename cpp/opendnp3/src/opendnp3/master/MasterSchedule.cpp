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

#include <openpal/Function1.h>

using namespace openpal;

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
	auto integrityTask = [this](ITask* pTask){ this->mpMaster->IntegrityPoll(pTask); };

	mpIntegrityPoll = mTracking.Add(
	                      arCfg.IntegrityRate,
	                      arCfg.TaskRetryRate,
	                      AMP_POLL,
			      Bind1<AsyncTaskBase*>(integrityTask),
	                      "Integrity Poll");

	mpIntegrityPoll->SetFlags(ONLINE_ONLY_TASKS | START_UP_TASKS);

	if (arCfg.DoUnsolOnStartup)
	{
		/*
		 * DNP3Spec-V2-Part2-ApplicationLayer-_20090315.pdf, page 8
		 * says that UNSOL should be disabled before an integrity scan
		 * is done.
		 */
		auto disableUnsol = [this](ITask* pTask){ this->mpMaster->ChangeUnsol(pTask, false, CLASS_1 | CLASS_2 | CLASS_3); };
	
		AsyncTaskBase* pUnsolDisable = mTracking.Add(
		                                   TimeDuration::Min(),
		                                   arCfg.TaskRetryRate,
		                                   AMP_UNSOL_CHANGE,
						   Bind1<AsyncTaskBase*>(disableUnsol),
		                                   "Unsol Disable");

		pUnsolDisable->SetFlags(ONLINE_ONLY_TASKS | START_UP_TASKS);
		mpIntegrityPoll->AddDependency(pUnsolDisable);

		if (arCfg.EnableUnsol)
		{
			auto enableUnsol = [this](ITask* pTask){ this->mpMaster->ChangeUnsol(pTask, true, CLASS_1 | CLASS_2 | CLASS_3); };

			AsyncTaskBase* pUnsolEnable = mTracking.Add(TimeDuration::Min(),
			                              arCfg.TaskRetryRate,
			                              AMP_UNSOL_CHANGE,
			                              Bind1<AsyncTaskBase*>(enableUnsol),
			                              "Unsol Enable");

			pUnsolEnable->SetFlags(ONLINE_ONLY_TASKS | START_UP_TASKS);
			pUnsolEnable->AddDependency(mpIntegrityPoll);
		}
	}

	/* Tasks are executed when the master is is idle */
	{
	auto lambda = [this](ITask* pTask){ mpMaster->ProcessCommand(pTask); };
	mpCommandTask = mTracking.AddContinuous(
	                    AMP_COMMAND,
			    Bind1<AsyncTaskBase*>(lambda),	                    
	                    "Command");
	}

	{
	auto lambda = [this](ITask* pTask){ mpMaster->SyncTime(pTask); };
	mpTimeTask = mTracking.AddContinuous(
	                 AMP_TIME_SYNC,
			 Bind1<AsyncTaskBase*>(lambda),
	                 "TimeSync");
	}

	{
	auto lambda = [this](ITask* pTask){ mpMaster->WriteIIN(pTask); };
	mpClearRestartTask = mTracking.AddContinuous(
	                         AMP_CLEAR_RESTART,
				 Bind1<AsyncTaskBase*>(lambda),
	                         "Clear IIN");
	}

	mpTimeTask->SetFlags(ONLINE_ONLY_TASKS);
	mpClearRestartTask->SetFlags(ONLINE_ONLY_TASKS);

}

AsyncTaskBase* MasterSchedule::AddClassScan(int classMask, TimeDuration aScanRate, TimeDuration aRetryRate)
{
	auto lambda = [this, classMask](ITask* pTask){ mpMaster->EventPoll(pTask, classMask); };

	auto pClassScan = mTracking.Add(	aScanRate,
	                                        aRetryRate,
	                                        AMP_POLL,
						Bind1<AsyncTaskBase*>(lambda),
	                                        "Class Scan");

	pClassScan->SetFlags(ONLINE_ONLY_TASKS);
	pClassScan->AddDependency(mpIntegrityPoll);
	return pClassScan;
}

}




