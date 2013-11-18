
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
// you under the terms of the License.
//

#ifndef __IO_SERVICE_THREAD_POOL_
#define __IO_SERVICE_THREAD_POOL_

#include <openpal/Visibility.h>
#include <openpal/Loggable.h>


#include "DeadlineTimerSteadyClock.h"

#include <boost/asio.hpp>

#include <thread>
#include <functional>


namespace asiopal
{

class DLL_LOCAL IOServiceThreadPool : private openpal::Loggable
{
public:

	IOServiceThreadPool(
	        openpal::Logger aLogger,
	        uint32_t aConcurrency,
	std::function<void()> onThreadStart = []() {},
	std::function<void()> onThreadExit = []() {}
	);

	~IOServiceThreadPool();

	boost::asio::io_service* GetIOService();

	void Shutdown();

private:

	std::function<void ()> mOnThreadStart;
	std::function<void ()> mOnThreadExit;

	bool mIsShutdown;

	void OnTimerExpiration(const boost::system::error_code& ec);

	void Run();

	boost::asio::io_service mService;
	boost::asio::monotonic_timer mInfiniteTimer;
	std::vector<std::thread*> mThreads;
};

}


#endif
