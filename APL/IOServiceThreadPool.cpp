
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

#include "Logger.h"
#include "LoggableMacros.h"

#include <chrono>

using namespace std;
using namespace std::chrono;

namespace apl
{

IOServiceThreadPool::IOServiceThreadPool(Logger* apLogger, size_t aConcurrency) :
	Loggable(apLogger),
	mService(),
	mInfiniteTimer(mService)
{
	assert(aConcurrency > 0);
	mInfiniteTimer.expires_at(high_resolution_clock::time_point::max());
	mInfiniteTimer.async_wait(bind(&IOServiceThreadPool::OnTimerExpiration, this, placeholders::_1));
	for(size_t i=0; i<aConcurrency; ++i) {		
		mThreads.push_back(new thread(bind(&IOServiceThreadPool::Run, this)));
	}
}

void IOServiceThreadPool::OnTimerExpiration(const boost::system::error_code& ec)
{
	
}

IOServiceThreadPool::~IOServiceThreadPool()
{
	for(auto pThread: mThreads) {		
		delete pThread;
	}
}

void IOServiceThreadPool::Shutdown()
{
	mInfiniteTimer.cancel();
	for(auto pThread: mThreads) pThread->join();
}

boost::asio::io_service* IOServiceThreadPool::GetIOService()
{
	return &mService;
}

void IOServiceThreadPool::Run()
{
	size_t num = 0;

	do {
		try {
			num = mService.run();
		}
		catch(const std::exception& ex) {
			LOG_BLOCK(LEV_ERROR, "Unhandled exception in thread pool: " << ex.what());
		}
	}
	while(num > 0);

	mService.reset();
}

}
