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
#include "IExecutor.h"

#include "AsyncResult.h"

#include <iostream>

using namespace std;
using namespace std::chrono;

namespace apl
{

IExecutor::IExecutor()
{

}

IExecutor::~IExecutor()
{

}

ITimer* IExecutor::Start(milliseconds aMs, const function<void ()>& arFunc)
{
	return this->Start(high_resolution_clock::duration(aMs), arFunc);
}

ITimer* IExecutor::StartInfinite()
{
	return this->Start(std::chrono::high_resolution_clock::time_point::max(), [](){});
}

void IExecutor::Synchronize(const std::function<void ()>& arFunc)
{
	AsyncResult ar;

	this->Post([arFunc, &ar](){
		try {
			arFunc();
			ar.Success();
		}
		catch(const std::exception& ex) {
			ar.Failure([ex](){ throw ex; });
		}
	});

	ar.Wait();
}


}
