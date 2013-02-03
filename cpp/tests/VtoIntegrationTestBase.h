
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
#ifndef __VTO_INTEGRATION_TEST_BASE_H_
#define __VTO_INTEGRATION_TEST_BASE_H_

#include <opendnp3/FlexibleDataObserver.h>
#include <opendnp3/LogToFile.h>
#include <opendnp3/Loggable.h>
#include <opendnp3/PhysicalLayerAsyncTCPClient.h>
#include <opendnp3/PhysicalLayerAsyncTCPServer.h>
#include <opendnp3/DNP3Manager.h>

#include "LogTester.h"
#include "AsyncTestObjectASIO.h"
#include "PhysicalLayerWrapper.h"
#include "WrappedTcpPipe.h"
#include "MockCommandHandler.h"


/** Platforms have different reserved port ranges */

#if defined(WIN32)
/* Windows platform */
#define MACRO_PORT_VALUE	(50000)
#else
/* Generic Linux platform */
#define MACRO_PORT_VALUE	(30000)
#endif

namespace opendnp3
{

class VtoIntegrationTestBase : public LogTester, protected Loggable
{
public:
	VtoIntegrationTestBase(
	        bool clientOnSlave = true,
	        bool aImmediateOutput = false,	        
	        FilterLevel level = LEV_INFO,
	        boost::uint16_t port = MACRO_PORT_VALUE);

	virtual ~VtoIntegrationTestBase();

	Logger* mpMainLogger;	
	MockCommandHandler cmdHandler;

	AsyncTestObjectASIO testObj;
	
	FlexibleDataObserver fdo;

	PhysicalLayerAsyncTCPClient vtoClient;
	PhysicalLayerAsyncTCPServer vtoServer;
	
	DNP3Manager mgr;
};

}

/* vim: set ts=4 sw=4: */

#endif

