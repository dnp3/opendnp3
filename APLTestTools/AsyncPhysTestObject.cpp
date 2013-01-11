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

#include "AsyncPhysTestObject.h"

namespace apl
{

AsyncPhysTestObject::AsyncPhysTestObject(FilterLevel aLevel, bool aImmediate, bool aAutoRead) :
	AsyncTestObjectASIO(),
	LogTester(aImmediate),
	mTCPClient(mLog.GetLogger(aLevel, "TCPClient"), this->GetService(), "127.0.0.1", 50000),
	mTCPServer(mLog.GetLogger(aLevel, "TCPSever"), this->GetService(), "127.0.0.1", 50000),
	mClientAdapter(mLog.GetLogger(aLevel, "ClientAdapter"), &mTCPClient, aAutoRead),
	mServerAdapter(mLog.GetLogger(aLevel, "ServerAdapter"), &mTCPServer, aAutoRead),
	mClientUpper(mLog.GetLogger(aLevel, "MockUpperClient")),
	mServerUpper(mLog.GetLogger(aLevel, "MockUpperServer"))
{
	mClientAdapter.SetUpperLayer(&mClientUpper);
	mServerAdapter.SetUpperLayer(&mServerUpper);
}

}
