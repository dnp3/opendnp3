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
#ifndef __LINK_CHANNEL_H_
#define __LINK_CHANNEL_H_

#include "LinkLayerRouter.h"

#include <vector>

namespace apl
{

class Logger;
class IPhysicalLayerAsync;
class IExecutor;
class AsyncTaskGroup;

namespace dnp
{

class Stack;

class LinkChannel : private LinkLayerRouter
{

	struct StackRecord {
		StackRecord() : pStack(NULL), route()
		{}

		StackRecord(Stack* apStack, const LinkRoute& arRoute) :
			pStack(apStack), route(arRoute)
		{}

		Stack* pStack;
		LinkRoute route;
	};

public:

	LinkChannel(Logger* apLogger, const std::string& arName, IExecutor* apExecutor, IPhysicalLayerAsync* apPhys, AsyncTaskGroup* apTaskGroup, millis_t aOpenRetry);

	void BindStackToChannel(const std::string& arStackName, Stack* apStack, const LinkRoute& arRoute);
	void RemoveStackFromChannel(const std::string& arStackName);
	std::vector<std::string> StacksOnChannel();

	std::string Name() {
		return mName;
	}

	void AddPhysicalLayerObserver(IPhysicalLayerObserver* apObserver) {
		this->AddObserver(apObserver);
	}

	AsyncTaskGroup* GetGroup() {
		return mpTaskGroup;
	}

	void BeginShutdown() {
		this->Shutdown();
	}

	void WaitUntilShutdown() {
		this->WaitForShutdown();
	}

private:

	std::string mName;
	AsyncTaskGroup* mpTaskGroup;

	typedef std::map<std::string, StackRecord> StackMap;
	StackMap mStackMap;

};

}
}


#endif

