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
#ifndef _MOCK_NOTIFIER_H_
#define _MOCK_NOTIFIER_H_

#include <APL/INotifier.h>

namespace apl
{

class MockNotifier : public INotifier
{
public:
	MockNotifier() {
		Reset();
	}

	void Notify() {
		mNotifications++;
	}

	void Reset() {
		mNotifications = 0;
	}

	size_t GetNotifications() {
		return mNotifications;
	};

private:
	size_t mNotifications;
};
}

#endif
