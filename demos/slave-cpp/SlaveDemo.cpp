
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
#include "SlaveDemo.h"

#include <APL/LoggableMacros.h>

using namespace std;

namespace apl
{
namespace dnp
{

SlaveDemoBase::SlaveDemoBase(Logger* apLogger) :
	IOService(),
	IOServiceThread(apLogger, this->Get()),
	mStrand(*Get()),
	mExecutor(&mStrand),
	mpInfiniteTimer(mExecutor.StartInfinite())
{
	// Create a notifier that when called will post a call to OnCommandNotify
	INotifier* pNotifier = mPostSource.Get(std::bind(&SlaveDemoBase::OnCommandNotify, this), &mExecutor);

	// Hand the notifier to the command source, so this happens whenever a new command is present
	mCommandQueue.SetNotifier(pNotifier);
}

void SlaveDemoBase::Shutdown()
{
	// this is the only outstanding event, so this will cause the
	// io_service thread to exit cleanly
	mpInfiniteTimer->Cancel();
}

void SlaveDemoBase::OnCommandNotify()
{
	// execute a single command
	mCommandQueue.ExecuteCommand(this);
}

SlaveDemoApp::SlaveDemoApp(Logger* apLogger) :
	SlaveDemoBase(apLogger),
	mCountSetPoints(0),
	mCountBinaryOutput(0),
	mpObserver(NULL)
{}

void SlaveDemoApp::SetDataObserver(IDataObserver* apObserver)
{
	mpObserver = apObserver;
}

CommandStatus SlaveDemoApp::HandleControl(Setpoint& aControl, size_t aIndex)
{
	LOG_BLOCK(LEV_INFO, "Received " << aControl.ToString() << " on index: " << aIndex);

	// Update a  feedback point that has the same value as the setpoint we were
	// given from the master. Configure it with the current time and good quality
	Analog a(aControl.GetValue(), AQ_ONLINE);
	a.SetToNow();

	// Create an additional counter to let the master know how many setpoints
	// we've receieved
	Counter c(++mCountSetPoints, CQ_ONLINE);
	c.SetToNow();

	// We would like all updates to be sent at one time.When the Transaction object
	// goes out of scope, all updates will be sent in one block to do the slave database.
	Transaction t(mpObserver);

	// Push the prepared datapoints to the database of this slave. The slave
	// can now transmit the changes to the master (polling or unsol)
	mpObserver->Update(a, aIndex);
	mpObserver->Update(c, 0);


	// This is the control code returned to the slave stack, and forwared
	// on to the master. These are DNP3 control codes.
	return CS_SUCCESS;
}

// same as for the setpoint
CommandStatus SlaveDemoApp::HandleControl(BinaryOutput& aControl, size_t aIndex)
{
	LOG_BLOCK(LEV_INFO, "Received " << aControl.ToString() << " on index: " << aIndex);

	// set the binary to ON if the command  code was LATCH_ON, otherwise set it off (LATCH_OFF)
	apl::Binary b(aControl.GetCode() == CC_LATCH_ON, BQ_ONLINE);
	b.SetToNow();

	// count how many BinaryOutput commands we recieve
	apl::Counter c(++mCountBinaryOutput, CQ_ONLINE);
	c.SetToNow();

	Transaction t(mpObserver);
	mpObserver->Update(b, aIndex);
	mpObserver->Update(c, 1);

	return CS_SUCCESS;
}

}
}
