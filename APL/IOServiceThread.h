//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __IO_SERVICE_THREAD_H_
#define __IO_SERVICE_THREAD_H_

#include "Thread.h"
#include "ASIOExecutor.h"
#include "Loggable.h"

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace apl
{

class Logger;
class Timer;

class IOServiceThread : public Threadable, protected Loggable
{
public:
	IOServiceThread(Logger* apLogger, boost::asio::io_service* apService);

	void Start() {
		mThread.Start();
	}
	void Stop();

	boost::asio::io_service* GetService() {
		return mpService;
	}

	void Run();
	void SignalStop();

protected:

	boost::asio::io_service* mpService;
	void Throw(); //need something to bind the hold timer to

private:

	Thread mThread;
};

}

#endif
