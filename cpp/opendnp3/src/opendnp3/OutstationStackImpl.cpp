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
#include "OutstationStackImpl.h"

namespace opendnp3
{

OutstationStackImpl::OutstationStackImpl(
        openpal::Logger& arLogger,
        openpal::IExecutor* apExecutor,
        ITimeWriteHandler* apTimeWriteHandler,
        ICommandHandler* apCmdHandler,
        const SlaveStackConfig& arCfg,		
		std::function<void (bool)> aEnableDisableFunc,
        std::function<void (IOutstation*)> aOnShutdown) :
	IOutstation(arLogger, aEnableDisableFunc),
	mpExecutor(apExecutor),
	mAppStack(arLogger, apExecutor, arCfg.app, arCfg.link),
	mDynamicDatabaseBuffer(arCfg.database),
	mDB(arLogger, mDynamicDatabaseBuffer.GetFacade()),
	mSlave(arLogger.GetSubLogger("outstation"), &mAppStack.mApplication, apExecutor, apTimeWriteHandler, &mDB, apCmdHandler, arCfg.slave),
	mOnShutdown(aOnShutdown)
{
	mAppStack.mApplication.SetUser(&mSlave);	
}

IDataObserver* OutstationStackImpl::GetDataObserver()
{
	return mSlave.GetDataObserver();
}

void OutstationStackImpl::SetNeedTimeIIN()
{
	mpExecutor->Post([this](){ this->mSlave.SetNeedTimeIIN(); });
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
	mOnShutdown(this);
}

}

