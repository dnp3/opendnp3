
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
#ifndef __STACK_HELPERS_H_
#define __STACK_HELPERS_H_

#include <APL/Log.h>
#include <APL/IOServiceThread.h>
#include <DNP3/AsyncStackManager.h>
#include <APL/CommandQueue.h>
#include <APL/FlexibleDataObserver.h>
#include <APL/QueueingFDO.h>
#include <APL/IPhysicalLayerAsync.h>
#include <APL/ASIOExecutor.h>
#include <APL/MultiplexingDataObserver.h>

#include <Terminal/Terminal.h>
#include <Terminal/LogTerminalExtension.h>
#include <Terminal/DOTerminalExtension.h>
#include <Terminal/ControlResponseTE.h>
#include <Terminal/ControlTerminalExtension.h>
#include <Terminal/FlexibleObserverTerminalExtension.h>

#include <XMLBindings/APLXML_MTS.h>
#include <XMLBindings/APLXML_STS.h>

#include <DNP3XML/XML_TestSet.h>
#include <APLXML/XMLConversion.h>

namespace apl
{
namespace dnp
{

class StackBase
{
public:
	StackBase(const APLXML_Base::PhysicalLayerList_t&, FilterLevel aLevel, const std::string& arLogFile = "-", bool aRemote = false, uint16_t aRemotePort = 4998);

	void Run();

protected:

	EventLog log;
	LogToFile logToFile;
	Logger* pTermLogger;

	boost::asio::io_service mService;
	IOServiceThread mTermThread;
	ASIOExecutor mExecutor;
	auto_ptr<IPhysicalLayerAsync> pTermPhys;

	QueueingFDO fdo;
	FlexibleObserverTerminalExtension fte;
	LogTerminalExtension lte;

	Terminal trm;
	AsyncStackManager mgr;

};

class SlaveXMLStack : public StackBase
{
public:
	SlaveXMLStack(APLXML_STS::SlaveTestSet_t* pCfg, FilterLevel aLevel);

	IDataObserver* GetDataObs() {
		return pObs;
	}

private:

	IDataObserver* pObs;
	MultiplexingDataObserver mdo;
	ControlResponseTE crte;
	DOTerminalExtension dote;
};

class MasterXMLStack : public StackBase
{
public:
	MasterXMLStack(APLXML_MTS::MasterTestSet_t* pCfg, FilterLevel aLevel);

	ICommandAcceptor* GetCmdAcceptor() {
		return accept;
	}

private:
	ICommandAcceptor* accept;
	ControlTerminalExtension cte;
};


}
}

#endif
