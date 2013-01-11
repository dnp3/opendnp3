
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
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
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

