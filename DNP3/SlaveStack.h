
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
#ifndef __SLAVE_STACK_H_
#define __SLAVE_STACK_H_

#include "Stack.h"
#include "Slave.h"
#include "Database.h"
#include "DNPCommandMaster.h"
#include "SlaveStackConfig.h"
#include <APL/TimeSource.h>

namespace apl
{
namespace dnp
{

/** @section desc A stack object for a master */
class SlaveStack : public Stack
{
public:

	/**
		@param apLogger			Logger used by the stack
		@param apTimeSrc		Source of system time
		@param apExecutor		Timer source used by the slave for asynchronous eventing
		@param apCmdAcceptor	Command acceptor interface used for dispatching commands to the outside world
		@param arCfg			Configuration struct that holds parameters for the stack
	*/
	SlaveStack(
	        Logger* apLogger,			
	        IExecutor* apExecutor,
	        ICommandAcceptor* apCmdAcceptor,
	        const SlaveStackConfig& arCfg);

	IVtoWriter* GetVtoWriter();

	IVtoReader* GetVtoReader();

	TimeSourceSystemOffset mTimeSource;
	Database mDB;				// The database holds static event data and forwards to an event buffer
	DNPCommandMaster mCmdMaster;	// Controls the execution of commands
	Slave mSlave;				// The dnp3 outstation class
};

}
}

#endif
