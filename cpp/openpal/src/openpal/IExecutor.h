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
#ifndef __I_EXECUTOR_H_
#define __I_EXECUTOR_H_

#include <functional>

#include "ITimer.h"
#include "TimeDuration.h"
#include "MonotonicTimestamp.h"

namespace openpal
{

/**
 * Interface for posting events to a queue.  Events can be posted for
 * immediate consumption or some time in the future.  Events can be consumbed
 * by the posting thread or another thread.
 *
 */
class IExecutor
{
public:

	virtual ~IExecutor() {}

	/** A non-absolute timestamp for the monotonic time source */
	virtual MonotonicTimestamp GetTime() = 0;

	/** Returns a new timer based on a relative time duration */
	virtual ITimer* Start(const TimeDuration& arDuration, const std::function<void ()> &) = 0;

	/** Returns a new timer based on an absolute timestamp of the monotonic clock */
	virtual ITimer* Start(const MonotonicTimestamp&, const std::function<void ()> &) = 0;

	/** Thread-safe way to post an event to be handled asynchronously */
	virtual void Post(const std::function<void ()> &) = 0;

};

}

#endif
