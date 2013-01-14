
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
#ifndef __SLAVE_DEMO_H_
#define __SLAVE_DEMO_H_

#include <APL/Log.h>
#include <APL/IOServiceThread.h>
#include <APL/CommandQueue.h>
#include <APL/FlexibleDataObserver.h>
#include <APL/IPhysicalLayerAsync.h>
#include <APL/IOService.h>
#include <APL/ASIOExecutor.h>
#include <APL/PostingNotifierSource.h>
#include <APL/LogToStdio.h>
#include <APL/Loggable.h>

#include <DNP3/SlaveStackConfig.h>
#include <DNP3/AsyncStackManager.h>

namespace apl
{
namespace dnp
{

/**
	This class takes care of all the plumbing and bingings between application code and the stack.
	Inherited clases on have to implement the ICommandHandler interface.
*/
class SlaveDemoBase : protected ICommandHandler, private IOService, public IOServiceThread
{
public:
	SlaveDemoBase(Logger* apLogger);

	// returns an interface the slave stack can use to notify when a command arrives
	ICommandAcceptor* GetCmdAcceptor() {
		return &mCommandQueue;
	}

	// Tell the io_service to exit
	void Shutdown();

private:

	/** OnCommandNotify has been marshalled to the application thread. It
		causes a single command to be processed */
	void OnCommandNotify();

	// The CommandQueue object serves as a thread-safe cache and notification mechanism. It implements the
	// ICommandAcceptor/IResponseAcceptor/ICommandSource interface, which is used by the slave/application
	// during control sequences
	CommandQueue mCommandQueue;

	PostingNotifierSource mPostSource;	// bridge between Notifiers and IO services that marshalls method calls between threads
	boost::asio::strand mStrand;		
	ASIOExecutor mExecutor;				// boost timer source, interface into the io service system
	ITimer* mpInfiniteTimer;			// timer used to keep the boost io service alive
	ICommandSource* mpCommandSource;	// The source for getting and executing commands.
};


/**
	This class demonstrates application logic, using APL scada types to respond to controls
	and updating data. In addition to the ICommandSource/ICommandHandler system, the
	IDataObserver interface gives transactional write capability to the slave's database.
*/
class SlaveDemoApp : public SlaveDemoBase
{
public:
	SlaveDemoApp(Logger* apLogger);

	void SetDataObserver(IDataObserver* apObserver);

	/**
		Processes a setpoint (Analog output) request from the master. This is where the user
		code will do some work. In this example, we'll just update the slave database.
		The return value is what the slave returns to the remote master.
	 */
	CommandStatus HandleControl(Setpoint& aControl, size_t aIndex);

	/**
		The same, but for binary outputs
	 */
	CommandStatus HandleControl(BinaryOutput& aControl, size_t aIndex);

private:
	int mCountSetPoints;    // count how many setpoints we recieve to demonstrate counters
	int mCountBinaryOutput; // count how many binary controls we recieve to demonstrate counters

	IDataObserver* mpObserver;  // The data sink for updating the slave database.
};

}
}

#endif
