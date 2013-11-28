
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "MasterAdapter.h"
#include "CommandProcessorAdapter.h"
#include "StackStateCallback.h"
#include "DeleteAnything.h"
#include "MasterScanAdapter.h"

namespace DNP3
{
namespace Adapter
{

MasterAdapter::MasterAdapter(opendnp3::IMaster* apMaster) :
	mpMaster(apMaster),
	mCommandAdapter(gcnew CommandProcessorAdapter(apMaster->GetCommandProcessor()))
{}

void MasterAdapter::AddStateListener(System::Action<StackState> ^ aListener)
{
	auto pListener = new gcroot < System::Action<StackState> ^ > (aListener);
	mpMaster->AddDestructorHook(std::bind(&DeleteAnything < gcroot < System::Action<StackState> ^ >> , pListener));
	mpMaster->AddStateListener(std::bind(&CallbackStackStateListener, std::placeholders::_1, pListener));
}

ICommandProcessor ^ MasterAdapter::GetCommandProcessor()
{
	return mCommandAdapter;
}

void MasterAdapter::Enable()
{
	mpMaster->Enable();
}

void MasterAdapter::Disable()
{
	mpMaster->Disable();
}

void MasterAdapter::Shutdown()
{
	mpMaster->Shutdown();
}

IMasterScan^ MasterAdapter::GetIntegrityScan()
{
	return gcnew MasterScanAdapter(mpMaster->GetIntegrityScan());
}

IMasterScan^ MasterAdapter::AddClassScan(int aClassMask, System::TimeSpan period, System::TimeSpan taskRetryPeriod)
{
	auto scan = mpMaster->AddClassScan(aClassMask, openpal::TimeDuration::Milliseconds(period.Milliseconds), openpal::TimeDuration::Milliseconds(taskRetryPeriod.Milliseconds));
	return gcnew MasterScanAdapter(scan);
}

}
}


