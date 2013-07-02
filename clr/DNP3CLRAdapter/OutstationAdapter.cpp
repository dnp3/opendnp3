
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

#include "OutstationAdapter.h"
#include "SlaveDataObserverAdapter.h"
#include "StackStateCallback.h"
#include "DeleteAnything.h"

namespace DNP3
{	
namespace Adapter
{	
	

OutstationAdapter::OutstationAdapter(opendnp3::IOutstation* apOutstation) :
	mpOutstation(apOutstation),
	mDataObserverAdapter(gcnew SlaveDataObserverAdapter(apOutstation->GetDataObserver()))
{}

void OutstationAdapter::AddStateListener(System::Action<StackState>^ aListener)
{
	auto pListener = new gcroot<System::Action<StackState>^>(aListener);
	mpOutstation->AddDestructorHook(std::bind(&DeleteAnything<gcroot<System::Action<StackState>^>>, pListener));
	mpOutstation->AddStateListener(std::bind(&CallbackStackStateListener, std::placeholders::_1, pListener));
}

IDataObserver^ OutstationAdapter::GetDataObserver()
{
	return mDataObserverAdapter;
}

void OutstationAdapter::Shutdown()
{
	mpOutstation->Shutdown();
}
		
}}
