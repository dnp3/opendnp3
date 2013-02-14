
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
#include "IOServiceThreadPool.h"

#include <opendnp3/Logger.h>
#include <opendnp3/Exception.h>

#include "LoggableMacros.h"


#include <chrono>

using namespace std;
using namespace std::chrono;

namespace opendnp3
{

IOServiceThreadPool::IOServiceThreadPool(
		Logger* apLogger, 
		uint32_t aConcurrency,
		std::function<void()> onThreadStart,
		std::function<void()> onThreadExit) :
	Loggable(apLogger),
	mOnThreadStart(onThreadStart),
	mOnThreadExit(onThreadExit),
	mIsShutdown(false),
	mService(),
	mInfiniteTimer(mService)
{	
	if(aConcurrency == 0) throw ArgumentException(LOCATION, "Concurrency cannot be 0");
	mInfiniteTimer.expires_at(steady_clock::time_point::max());
	mInfiniteTimer.async_wait(bind(&IOServiceThreadPool::OnTimerExpiration, this, placeholders::_1));
	for(uint32_t i=0; i<aConcurrency; ++i) {
		mThreads.push_back(new thread(bind(&IOServiceThreadPool::Run, this)));
	}
}

void IOServiceThreadPool::OnTimerExpiration(const boost::system::error_code& ec)
{
	
}

IOServiceThreadPool::~IOServiceThreadPool()
{
	this->Shutdown();
	for(auto pThread: mThreads) {
		delete pThread;
	}
}

void IOServiceThreadPool::Shutdown()
{
	if(!mIsShutdown) {
		mIsShutdown = true;
		mInfiniteTimer.cancel();	
		for(auto pThread: mThreads) pThread->join();	
	}
}

boost::asio::io_service* IOServiceThreadPool::GetIOService()
{
	return &mService;
}

void IOServiceThreadPool::Run()
{
	size_t num = 0;

	mOnThreadStart();

	do {
		try {
			num = mService.run();			
		}
		catch(const std::exception& ex) {
			num = 0;
			LOG_BLOCK(LEV_ERROR, "Unhandled exception in thread pool: " << ex.what());
		}
	}
	while(num > 0);

	mOnThreadExit();
}

}
