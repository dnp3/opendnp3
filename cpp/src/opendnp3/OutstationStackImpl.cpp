
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

#include "OutstationStackImpl.h"

namespace opendnp3
{

OutstationStackImpl::OutstationStackImpl(
        Logger* apLogger,
        boost::asio::io_service* apService,
        IExecutor* apExecutor,
        ICommandHandler* apCmdHandler,
        const SlaveStackConfig& arCfg,
        std::function<void (IOutstation*)> aOnShutdown) :
	IOutstation(apLogger, apService),
	mpExecutor(apExecutor),
	mAppStack(apLogger, apExecutor, arCfg.app, arCfg.link),
	mDB(apLogger),
	mSlave(apLogger->GetSubLogger("outstation"), &mAppStack.mApplication, apExecutor, &mTimeSource, &mDB, apCmdHandler, arCfg.slave),
	mOnShutdown(aOnShutdown)
{
	mAppStack.mApplication.SetUser(&mSlave);
	mDB.Configure(arCfg.device);
}

IDataObserver* OutstationStackImpl::GetDataObserver()
{
	return mSlave.GetDataObserver();
}

ILinkContext* OutstationStackImpl::GetLinkContext()
{
	return &mAppStack.mLink;
}

void OutstationStackImpl::SetLinkRouter(ILinkRouter* apRouter)
{
	mAppStack.mLink.SetRouter(apRouter);
}

void OutstationStackImpl::AddStateListener(std::function<void (StackState)> aListener)
{
	mSlave.AddStateListener(aListener);
}

void OutstationStackImpl::Shutdown()
{
	this->CleanupVto();
	mOnShutdown(this);
}

IVtoWriter* OutstationStackImpl::GetVtoWriter()
{
	return mSlave.GetVtoWriter();
}

IVtoReader* OutstationStackImpl::GetVtoReader()
{
	return mSlave.GetVtoReader();
}

}

