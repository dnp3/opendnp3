/*
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
#include "asiopal/ThreadPool.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

#include <chrono>
#include <sstream>

#include <asiopal/SteadyClock.h>

using namespace std;
using namespace std::chrono;
using namespace openpal;

namespace asiopal
{

ThreadPool::ThreadPool(
    openpal::ILogHandler* handler,
    uint32_t levels,
    uint32_t concurrency,
    std::function<void()> onThreadStart_,
    std::function<void()> onThreadExit_) :
	root(handler, "pool", levels),
	onThreadStart(onThreadStart_),
	onThreadExit(onThreadExit_),
	isShutdown(false),
	ioservice(std::make_shared<asio::io_service>()),
	infiniteTimer(*ioservice)
{
	if(concurrency == 0)
	{
		concurrency = 1;
		SIMPLE_LOG_BLOCK(root.logger, logflags::WARN, "Concurrency was set to 0, defaulting to 1 thread");
	}
	infiniteTimer.expires_at(asiopal::asiopal_steady_clock::time_point::max());
	infiniteTimer.async_wait([](const std::error_code&) {});
	for(uint32_t i = 0; i < concurrency; ++i)
	{
		threads.push_back(new thread(bind(&ThreadPool::Run, this)));
	}
}

std::shared_ptr<ThreadPool> ThreadPool::Create(
    openpal::ILogHandler* handler,
    uint32_t levels,
    uint32_t concurrency,
    std::function<void()> onThreadStart,
    std::function<void()> onThreadExit)
{
	return std::make_shared<ThreadPool>(handler, levels, concurrency, onThreadStart, onThreadExit);
}

ThreadPool::~ThreadPool()
{
	this->Shutdown();
	for(auto pThread : threads)
	{
		delete pThread;
	}
}

void ThreadPool::Shutdown()
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

asio::io_service& ThreadPool::GetIOService()
{
	return *ioservice;
}

void ThreadPool::Run()
{
	onThreadStart();
	ioservice->run();
	onThreadExit();
}

}
