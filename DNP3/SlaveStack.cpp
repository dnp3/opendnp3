//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "SlaveStack.h"

namespace apl
{
namespace dnp
{

SlaveStack::SlaveStack(Logger* apLogger, IExecutor* apExecutor, ICommandAcceptor* apCmdAcceptor, const SlaveStackConfig& arCfg) :
	Stack(apLogger->GetSubLogger("slave"), apExecutor, arCfg.app, arCfg.link),
	mDB(apLogger),
	mCmdMaster(&mTimeSource, 10000),
	mSlave(apLogger, &mApplication, apExecutor, &mTimeSource, &mDB, &mCmdMaster, arCfg.slave)
{
	this->mApplication.SetUser(&mSlave);
	mDB.Configure(arCfg.device);
	mCmdMaster.Configure(arCfg.device, apCmdAcceptor);
}

IVtoWriter* SlaveStack::GetVtoWriter()
{
	return this->mSlave.GetVtoWriter();
}

IVtoReader* SlaveStack::GetVtoReader()
{
	return this->mSlave.GetVtoReader();
}

}
}