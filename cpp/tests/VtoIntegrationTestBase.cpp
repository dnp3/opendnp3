
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

#include "VtoIntegrationTestBase.h"

#include <opendnp3/LogToStdio.h>
#include <opendnp3/SlaveStackConfig.h>
#include <opendnp3/MasterStackConfig.h>
#include <opendnp3/VtoRouterSettings.h>
#include <opendnp3/IChannel.h>
#include <opendnp3/IMaster.h>
#include <opendnp3/IOutstation.h>
#include <opendnp3/ITimeWriteHandler.h>

#include <thread>

namespace opendnp3
{

VtoIntegrationTestBase::VtoIntegrationTestBase(
        bool clientOnSlave,
        bool aImmediateOutput,
        FilterLevel level,
        boost::uint16_t port) :
		
	log(),
	testObj(),
	vtoClient(Logger(&log, level, "local-tcp-client"), testObj.GetService(), "127.0.0.1", port + 20),
	vtoServer(Logger(&log, level, "loopback-tcp-server"), testObj.GetService(), "127.0.0.1", port + 10),
	mgr(std::thread::hardware_concurrency())
{

	if(aImmediateOutput) {
		mgr.AddLogSubscriber(LogToStdio::Inst());		
	}


	auto pServerChannel = mgr.AddTCPServer("dnp-tcp-server", level, 5000, "127.0.0.1", port);
	SlaveStackConfig sconfig;
	sconfig.app.NumRetry = 3;
	sconfig.app.RspTimeout = 500;
	auto pOutstation = pServerChannel->AddOutstation("outstation", level, &cmdHandler, NullTimeWriteHandler::Inst(), sconfig);



	auto pClientChannel = mgr.AddTCPClient("dnp-tcp-client", level, 5000, "127.0.0.1", port);
	MasterStackConfig mconfig;
	mconfig.app.NumRetry = 3;
	mconfig.app.RspTimeout = 500;
	mconfig.master.UseNonStandardVtoFunction = true;
	auto pMaster = pClientChannel->AddMaster("master", level, &fdo, mconfig);


	// switch if master or slave gets the loopback half of the server

	auto pStack1 = clientOnSlave ? ((IStack*) pOutstation) : ((IStack*) pMaster);
	auto pStack2 = clientOnSlave ? ((IStack*) pMaster) : ((IStack*) pOutstation);

	pStack1->StartVtoRouterTCPClient("vto-tcp-client", level, "127.0.0.1", port + 10, VtoRouterSettings(88, false, false, 1000));
	pStack2->StartVtoRouterTCPServer("vto-tcp-server", level, "127.0.0.1", port + 20, VtoRouterSettings(88, true, false, 1000));
}

VtoIntegrationTestBase::~VtoIntegrationTestBase()
{

}

}

/* vim: set ts=4 sw=4: */
