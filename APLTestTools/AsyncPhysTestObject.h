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
#ifndef __ASYNC_PHYS_TEST_OBJECT_H_
#define __ASYNC_PHYS_TEST_OBJECT_H_

#include <APLTestTools/AsyncTestObjectASIO.h>

#include <APLTestTools/LogTester.h>
#include <APL/PhysicalLayerAsyncTCPClient.h>
#include <APL/PhysicalLayerAsyncTCPServer.h>
#include <APL/LowerLayerToPhysAdapter.h>
#include <APLTestTools/MockUpperLayer.h>

namespace apl
{

class AsyncPhysTestObject : public AsyncTestObjectASIO, public LogTester
{
public:
	AsyncPhysTestObject(FilterLevel aLevel = LEV_INFO, bool aImmediate = false, bool aAutoRead = true);

private:
	Logger* mpLogger;

public:
	PhysicalLayerAsyncTCPClient mTCPClient;
	PhysicalLayerAsyncTCPServer mTCPServer;

	LowerLayerToPhysAdapter mClientAdapter;
	LowerLayerToPhysAdapter mServerAdapter;

	MockUpperLayer mClientUpper;
	MockUpperLayer mServerUpper;
};

}

#endif
