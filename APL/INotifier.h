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
#ifndef __I_NOTIFIER_H_
#define __I_NOTIFIER_H_

namespace apl
{
/**
	A generic interface that defines a notification function. Modeled loosly on a lock
	type interaction. Generally used for the multi-threaded observer pattern to indicate
	that a shared _thread_safe_ object has had a change occur. The application then knows
	that it needs to look at that shared object which it can do immediatley or at some point
	in the future (ie. when the "application" thread is ready).

	It is critical to remember that the calls to Notify() can potentially come from any thread!
 */
class INotifier
{
public:
	virtual ~INotifier() {}
	virtual void Notify() = 0;
};
}

#endif

