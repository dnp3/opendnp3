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
#ifndef ASIOPAL_IOSERVICETHREADPOOL_H
#define ASIOPAL_IOSERVICETHREADPOOL_H

#include <openpal/logging/LogRoot.h>

#include <asio.hpp>

#include <functional>
#include <thread>

namespace asiopal
{

class IOServiceThreadPool
{
public:

	IOServiceThreadPool(
	    openpal::ILogHandler* pHandler,
	    uint32_t levels,		
	    uint32_t aConcurrency,
	std::function<void()> onThreadStart = []() {},
	std::function<void()> onThreadExit = []() {}
	);

	~IOServiceThreadPool();

	asio::io_service& GetIOService();

	void Shutdown();

private:

	openpal::LogRoot root;
	openpal::Logger logger;

	std::function<void ()> onThreadStart;
	std::function<void ()> onThreadExit;

	bool isShutdown;	

	void Run();

	asio::io_service ioservice;
	asio::basic_waitable_timer< std::chrono::steady_clock > infiniteTimer;
	std::vector<std::thread*> threads;
};

}


#endif
