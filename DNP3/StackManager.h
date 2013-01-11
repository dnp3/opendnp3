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
#ifndef __STACK_MANAGER_H_
#define __STACK_MANAGER_H_

#include <APL/PhysLayerSettings.h>
#include <APL/DataInterfaces.h>
#include <APL/CommandInterfaces.h>
#include <APL/LogBase.h>
#include <APL/SerialTypes.h>

#include <DNP3/MasterStackConfig.h>
#include <DNP3/SlaveStackConfig.h>

#include <vector>

namespace apl
{
class EventLog;
class ILogBase;
}

namespace apl
{
namespace dnp
{

class AsyncStackManager;

/** Wraps the AyncStackManger using the impl pattern. This makes it suiteable
	for wrapping with swig or for creating a header-only distribution
	The functions do the exact same thing as their impl counterparts
	*/
class StackManager
{
public:
	StackManager();
	~StackManager();

	void AddTCPClient(const std::string& arName, PhysLayerSettings aPhys, const std::string& arAddr, uint16_t aPort);
	void AddTCPServer(const std::string& arName, PhysLayerSettings  aPhys, const std::string& arEndpoint, uint16_t aPort);
	void AddSerial(const std::string& arName, PhysLayerSettings aPhys, SerialSettings aSerial);

	ICommandAcceptor* AddMaster(const std::string& arPortName,
	                            const std::string& arStackName,
	                            FilterLevel aLevel,
	                            IDataObserver* apPublisher,
	                            const MasterStackConfig& arCfg);

	IDataObserver* AddSlave(const std::string& arPortName,
	                        const std::string& arStackName,
	                        FilterLevel aLevel,
	                        ICommandAcceptor* apCmdAcceptor,
	                        const SlaveStackConfig& arCfg);

	void RemovePort(const std::string& arPortName);

	void RemoveStack(const std::string& arStackName);

	void AddLogHook(ILogBase*);

	std::vector<std::string> GetStackNames();

	std::vector<std::string> GetPortNames();


	void Shutdown();

private:
	EventLog* mpLog;
	AsyncStackManager* mpImpl;
};

}
}

#endif

