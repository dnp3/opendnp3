
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

#include "ChannelAdapter.h"
#include "Conversions.h"
#include "MasterDataObserverAdapter.h"
#include "SlaveCommandHandlerAdapter.h"
#include "MasterAdapter.h"
#include "OutstationAdapter.h"
#include "DeleteAnything.h"

using namespace System::Collections::Generic;

namespace DNP3
{	
namespace Adapter
{	


ChannelAdapter::ChannelAdapter(opendnp3::IChannel* apChannel) : 
	mpChannel(apChannel)	
{
	
}

void ChannelAdapter::AddStateListener(System::Action<ChannelState>^ listener)
{
	auto pWrapper = new gcroot<System::Action<ChannelState>^>(listener);
	mpChannel->AddDestructorHook(std::bind(&DeleteAnything<gcroot<System::Action<ChannelState>^>>, pWrapper));
	mpChannel->AddStateListener(std::bind(&CallbackListener, pWrapper, std::placeholders::_1));
}

void CallbackListener(gcroot<System::Action<ChannelState>^>* listener, opendnp3::ChannelState aState)
{
	ChannelState state = Conversions::convertChannelState(aState);
	(*listener)->Invoke(state);
}

IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, LogLevel level, IDataObserver^ publisher, MasterStackConfig^ config)
{
	std::string stdLoggerId = Conversions::convertString(loggerId);	
	openpal::FilterLevel stdLevel = Conversions::convertFilterLevel(level);

	MasterDataObserverWrapper^ wrapper = gcnew MasterDataObserverWrapper(publisher);
	opendnp3::MasterStackConfig cfg = Conversions::convertConfig(config);

	try {
		auto pMaster = mpChannel->AddMaster(stdLoggerId, stdLevel, wrapper->Get(), cfg);
		return gcnew MasterAdapter(pMaster);
	} 
	catch(openpal::Exception ex){
		throw Conversions::convertException(ex);
	}
}

IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, LogLevel level, ICommandHandler^ cmdHandler, SlaveStackConfig^ config)
{	
	std::string stdLoggerId = Conversions::convertString(loggerId);
	openpal::FilterLevel stdLevel = Conversions::convertFilterLevel(level);

	SlaveCommandHandlerWrapper^ wrapper = gcnew SlaveCommandHandlerWrapper(cmdHandler);
	opendnp3::SlaveStackConfig cfg = Conversions::convertConfig(config);

	try {
		auto pOutstation = mpChannel->AddOutstation(stdLoggerId, stdLevel, wrapper->Get(), Conversions::convertConfig(config));
		return gcnew OutstationAdapter(pOutstation);
	} 
	catch(openpal::Exception ex){
		throw Conversions::convertException(ex);
	}
}

void ChannelAdapter::Shutdown()
{
	mpChannel->Shutdown();
}
			
}}


