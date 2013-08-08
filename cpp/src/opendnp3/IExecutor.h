
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

#ifndef __I_EXECUTOR_H_
#define __I_EXECUTOR_H_

#include "ITimer.h"

#include <chrono>
#include <functional>


#include <opendnp3/Visibility.h>

namespace opendnp3
{

/**
 * Interface for posting events to a queue.  Events can be posted for
 * immediate consumption or some time in the future.  Events can be consumbed
 * by the posting thread or another thread.
 *
 * @section Usage
 *
 * \code
 * 	 asio::io_service srv;
 * 	 ASIOExecutor timers(&srv);
 * \endcode
 *
 * @see TimerASIO
 */
class DLL_LOCAL IExecutor
{
public:

	virtual ~IExecutor() {}

	/** Returns a new timer based on a relative time in milliseconds */
	ITimer* Start(std::chrono::milliseconds, const std::function<void ()> &);

	/** Returns a new timer based on a relative time duration */
	virtual ITimer* Start(openpal::timer_clock::duration, const std::function<void ()> &) = 0;

	/** Returns a new timer based on an absolute time */
	virtual ITimer* Start(const openpal::timer_clock::time_point&, const std::function<void ()> &) = 0;

	/** Thread-safe way to post an event to be handled asynchronously */
	virtual void Post(const std::function<void ()> &) = 0;

};

}

#endif
