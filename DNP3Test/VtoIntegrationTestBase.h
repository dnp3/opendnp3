/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */
#ifndef __VTO_INTEGRATION_TEST_BASE_H_
#define __VTO_INTEGRATION_TEST_BASE_H_

#include <APLTestTools/LogTester.h>
#include <APLTestTools/MockCommandAcceptor.h>
#include <APLTestTools/AsyncTestObjectASIO.h>
#include <APLTestTools/PhysicalLayerWrapper.h>
#include <APLTestTools/WrappedTcpPipe.h>

#include <APL/FlexibleDataObserver.h>
#include <APL/LogToFile.h>
#include <APL/Loggable.h>
#include <APL/PhysicalLayerAsyncTCPClient.h>
#include <APL/PhysicalLayerAsyncTCPServer.h>

#include <DNP3/AsyncStackManager.h>


/** Platforms have different reserved port ranges */

#if defined(WIN32)
/* Windows platform */
#define MACRO_PORT_VALUE	(50000)
#else
/* Generic Linux platform */
#define MACRO_PORT_VALUE	(30000)
#endif

namespace apl
{
namespace dnp
{

class VtoIntegrationTestBase : public LogTester, protected Loggable
{
public:
	VtoIntegrationTestBase(
	        bool clientOnSlave = true,
	        bool aImmediateOutput = false,
	        bool aLogToFile = false,
	        FilterLevel level = LEV_INFO,
	        boost::uint16_t port = MACRO_PORT_VALUE);

	virtual ~VtoIntegrationTestBase();

	Logger* mpMainLogger;
	std::auto_ptr<LogToFile> mpLtf;
	MockCommandAcceptor cmdAcceptor;

	AsyncTestObjectASIO testObj;
	
	FlexibleDataObserver fdo;

	ASIOExecutor timerSource;
	PhysicalLayerAsyncTCPClient vtoClient;
	PhysicalLayerAsyncTCPServer vtoServer;
	
	AsyncStackManager manager;
	WrappedTcpPipe tcpPipe;
};

}

}

/* vim: set ts=4 sw=4: */

#endif
