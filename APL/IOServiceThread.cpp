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

#include "IOServiceThread.h"

#include "Exception.h"
#include "Logger.h"
#include "LoggableMacros.h"

#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;

namespace apl
{

class IOServiceExitException : public Exception
{
public:
	IOServiceExitException(const std::string& aSource) throw() :
		Exception(aSource, "")
	{}
};

IOServiceThread::IOServiceThread(Logger* apLogger, boost::asio::io_service* apService) :
	Loggable(apLogger),
	mpService(apService),
	mThread(this)
{

}

void IOServiceThread::Stop()
{
	mThread.RequestStop();
	mThread.WaitForStop();
}

void IOServiceThread::Throw()
{
	throw IOServiceExitException(LOCATION);
}

void IOServiceThread::Run()
{
	size_t num = 0;

	try {
		num = mpService->run();
	}
	catch(IOServiceExitException&) {
		LOG_BLOCK(LEV_INFO, "IOService exited via IOServiceExitException");
	}
	catch(const std::exception& ex) {
		LOG_BLOCK(LEV_ERROR, "Unexpected exception: " << ex.what());
	}

}

void IOServiceThread::SignalStop()
{
	mpService->post(std::bind(&IOServiceThread::Throw, this));
}

}
