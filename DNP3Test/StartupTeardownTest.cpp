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

#include "StartupTeardownTest.h"

#include <DNP3/MasterStack.h>
#include <APL/PhysicalLayerAsyncTCPClient.h>
#include <APL/LogToStdio.h>

#include <boost/asio.hpp>


using namespace apl;
using namespace apl::dnp;

StartupTeardownTest::StartupTeardownTest(FilterLevel aLevel, bool aImmediate) :
	log(),
	manager(log.GetLogger(aLevel, "mgr"))
{
	if(aImmediate) log.AddLogSubscriber(LogToStdio::Inst());
}

void StartupTeardownTest::CreatePort(const std::string& arName, FilterLevel aLevel)
{
	std::string name = arName + " router";
	PhysLayerSettings s(aLevel, 1000);
	manager.AddTCPClient(arName, s, "127.0.0.1", 30000);
}

void StartupTeardownTest::AddMaster(const std::string& arStackName, const std::string& arPortName, boost::uint16_t aLocalAddress, FilterLevel aLevel)
{
	MasterStackConfig cfg;
	cfg.link.LocalAddr = aLocalAddress;
	manager.AddMaster(arPortName, arStackName, aLevel, &fdo, cfg);
}

