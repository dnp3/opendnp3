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
#ifndef __ASYNC_TEST_OBJECT_ASIO_H_
#define __ASYNC_TEST_OBJECT_ASIO_H_

#include <APLTestTools/AsyncTestObject.h>

namespace boost
{
namespace asio
{
class io_service;
}
}

namespace apl
{

class AsyncTestObjectASIO : public AsyncTestObject
{
public:
	AsyncTestObjectASIO();
	AsyncTestObjectASIO(boost::asio::io_service*);
	virtual ~AsyncTestObjectASIO();

	boost::asio::io_service* GetService() {
		return mpTestObjectService;
	}

	static void Next(boost::asio::io_service* apSrv, millis_t aSleep);

private:
	boost::asio::io_service* mpTestObjectService;
	bool mOwner;
	void Next();

};

}

#endif
