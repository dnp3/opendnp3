
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
#include "MasterDemo.h"

#include <APL/LoggableMacros.h>

using namespace std;

namespace apl { namespace dnp {

MasterDemoBase::MasterDemoBase(Logger* apLogger)
	: IOService()
	, IOServiceThread(apLogger, this->Get())
	, mStrand(*Get())
	, mExecutor(&mStrand)
	, mpInfiniteTimer(mExecutor.StartInfinite())
	, mTestCounter(0)
{
	/*
	 * Create a notifier that will be called whenever data updates
	 * are received.
	 */
	INotifier* pNotifier =
		mPostSource.Get(
			std::bind(
				&MasterDemoBase::OnDataUpdate,
				this
			),
			&mExecutor
		);

	/*
	 * Register the INotifier with the QueueingFDO instance.
	 */
	mFDO.AddObserver(pNotifier);
}

void MasterDemoBase::Shutdown()
{
	// this is the only outstanding event, so this will cause the
	// io_service thread to exit cleanly
	mpInfiniteTimer->Cancel();
}

void MasterDemoBase::OnDataUpdate()
{
	// Obtain a lock on the QueueingFDO
	Transaction tr(&mFDO);

	// Print the contents of the update queue
	size_t num_updates = mFDO.updates.size();
	if ( num_updates > 0 ) {
		LOG_BLOCK(LEV_INFO,
				"Received " << num_updates
				<< " updates");
		do {
			LOG_BLOCK(LEV_INFO, mFDO.updates.front());
			mFDO.updates.pop_front();
		} while ( !mFDO.updates.empty() );
	}

	if ( num_updates > 1 ) {
		// Increment the values on the Setpoints (Analog Outputs)
		typedef PointMap<SetpointStatus>::Type SetpointMap;
		typedef SetpointMap::iterator SetpointMapIter;
		int index = 0;
		SetpointMapIter iter = mFDO.mSetpointStatusMap.begin();
		for (; iter != mFDO.mSetpointStatusMap.end(); ++iter, ++index)
		{
			const int16_t value = ++mTestCounter;
			if ( mTestCounter < 0 )
				mTestCounter = 0;

			LOG_BLOCK(LEV_DEBUG, "Assigning new value "
					<< value
					<< " to index "
					<< index);

			Setpoint sp(value);
			mpCommandAcceptor->AcceptCommand(
				sp,    // Setpoint instance
				index, // point index
				index, // sequence value
				this   // IResponseAcceptor*
			);
		}
		LOG_BLOCK(LEV_INFO,
				"Sent " << index
				<< " updates");
	}
}

void MasterDemoBase::AcceptResponse(const CommandResponse& aResponse,
		int aSequence)
{
	LOG_BLOCK(LEV_DEBUG, "Response received for update to index "
			<< aSequence << ": "
			<< ToString(aResponse.mResult));
}

MasterDemoApp::MasterDemoApp(Logger* apLogger)
	: MasterDemoBase(apLogger)
{}

void MasterDemoApp::OnStateChange(StackStates aState)
{
	LOG_BLOCK(LEV_INFO, "Communications link state change: "
			<< ConvertStackStateToString(aState));
}

}} // end namespaces apl and apl::dnp

