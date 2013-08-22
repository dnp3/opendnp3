
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

#include "ExecutorPause.h"

#include <openpal/IExecutor.h>

using namespace openpal;

namespace opendnp3
{

ExecutorPause::ExecutorPause(IExecutor* apExecutor) :
	mpExecutor(apExecutor),
	mPaused(false),
	mComplete(false),
	mExit(false)
{
	mpExecutor->Post([this]() {
		this->Pause();
	});
	std::unique_lock<std::mutex> lock(mMutex);
	while(!mPaused) mCondition.wait(lock);
}

ExecutorPause::~ExecutorPause()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mComplete = true;
	mCondition.notify_one();
	while(!mExit) mCondition.wait(lock);
}

void ExecutorPause::Pause()
{
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mPaused = true;
		mCondition.notify_one();
		while(!mComplete) mCondition.wait(lock);
	}
	mExit = true; // make sure we're not going to touch from other thread after destruction
	mCondition.notify_one();
}

}


