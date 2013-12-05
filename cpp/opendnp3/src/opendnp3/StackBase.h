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
#ifndef __STACK_BASE_H_
#define __STACK_BASE_H_

#include <openpal/Visibility.h>
#include <opendnp3/gen/StackState.h>

#include <functional>
#include <vector>

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

class DLL_LOCAL StackBase
{
public:
	StackBase(openpal::IExecutor* apExecutor);

	void AddStateListener(std::function<void (StackState)> aCallback);

protected:
	void NotifyListeners(StackState aState);

	void Queue(const std::function<void (StackState)>& arFunc, StackState aState);

	// implement in inherited class
	virtual StackState GetState() = 0;

	openpal::IExecutor* mpExecutor;

private:
	std::vector<std::function<void (StackState)>> mListeners;
};
}

#endif
