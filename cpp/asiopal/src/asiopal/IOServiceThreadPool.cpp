/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "IOServiceThreadPool.h"

#include <openpal/LoggableMacros.h>
#include <openpal/LogLevels.h>

#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;
using namespace openpal;

#ifdef ERROR
#undef ERROR
#endif

namespace asiopal
{

IOServiceThreadPool::IOServiceThreadPool(
    ILogBase* pLog,
    uint32_t levels,
    const std::string& id,
    uint32_t aConcurrency,
    std::function<void()> onThreadStart_,
    std::function<void()> onThreadExit_) :
	root(pLog, levels),
	logger(root.GetLogger(id)),
	onThreadStart(onThreadStart_),
	onThreadExit(onThreadExit_),
	isShutdown(false),
	ioservice(),
	infiniteTimer(ioservice)
{
	if(aConcurrency == 0)
	{
		aConcurrency = 1;
		LOG_BLOCK(log::WARN, "Concurrency was set to 0, defaulting to 1 thread");
	}
	infiniteTimer.expires_at(std::chrono::steady_clock::time_point::max());
	infiniteTimer.async_wait(bind(&IOServiceThreadPool::OnTimerExpiration, this, placeholders::_1));
	for(uint32_t i = 0; i < aConcurrency; ++i)
	{
		threads.push_back(new thread(bind(&IOServiceThreadPool::Run, this)));
	}
}

void IOServiceThreadPool::OnTimerExpiration(const std::error_code& ec)
{

}

IOServiceThreadPool::~IOServiceThreadPool()
{
	this->Shutdown();
	for(auto pThread : threads)
	{
		delete pThread;
	}
}

void IOServiceThreadPool::Shutdown()
{
	if(!isShutdown)
	{
		isShutdown = true;
		infiniteTimer.cancel();
		for (auto pThread : threads)
		{
			pThread->join();
		}
	}
}

asio::io_service* IOServiceThreadPool::GetIOService()
{
	return &ioservice;
}

void IOServiceThreadPool::Run()
{
	size_t num = 0;

	onThreadStart();

	do
	{
		try
		{
			num = ioservice.run();
		}
		catch(const std::exception& ex)
		{
			num = 1;
			LOG_BLOCK(log::ERR, "Unhandled exception in thread pool: " << ex.what());
		}
	}
	while(num > 0);

	onThreadExit();
}

}
