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

#include "AsyncTestObjectASIO.h"
#include <boost/asio.hpp>

#include <APL/Location.h>
#include <APL/Exception.h>
#include <thread>
#include <chrono>

namespace apl
{

AsyncTestObjectASIO::AsyncTestObjectASIO() :
	mpTestObjectService(new boost::asio::io_service()),
	mOwner(true)
{}

AsyncTestObjectASIO::AsyncTestObjectASIO(boost::asio::io_service* apService) :
	mpTestObjectService(apService),
	mOwner(false)
{

}

AsyncTestObjectASIO::~AsyncTestObjectASIO()
{
	if(mOwner) delete mpTestObjectService;
}

void AsyncTestObjectASIO::Next()
{
	Next(this->GetService(), 10);
}

void AsyncTestObjectASIO::Next(boost::asio::io_service* apSrv, millis_t aSleep)
{
	boost::system::error_code ec;
	size_t num = apSrv->poll_one(ec);
	if(ec) throw Exception(LOCATION, ec.message());
	if(num == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(aSleep));
	}
	apSrv->reset();
}



}
