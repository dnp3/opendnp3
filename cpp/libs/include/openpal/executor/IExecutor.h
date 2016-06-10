/*
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
#ifndef OPENPAL_IEXECUTOR_H
#define OPENPAL_IEXECUTOR_H

#include "ITimer.h"
#include "TimeDuration.h"
#include "Action0.h"
#include "IMonotonicTimeSource.h"


namespace openpal
{

/**
 * Interface for posting events to a queue.  Events can be posted for
 * immediate consumption or some time in the future.  Events are processed
 * in the order they are received.
 *
 */
class IExecutor : public IMonotonicTimeSource
{

public:

	virtual ~IExecutor() {}

	/// @return a new timer based on a relative time duration
	virtual ITimer* Start(const TimeDuration& duration, const Action0& action) = 0;

	/// @return a new timer based on an absolute timestamp of the monotonic clock
	virtual ITimer* Start(const MonotonicTimestamp& expiration, const Action0& action) = 0;

	/// @return Thread-safe way to post an event to be handled asynchronously
	virtual void Post(const Action0& action) = 0;

	template <class Lambda>
	void PostLambda(Lambda& lambda)
	{
		this->Post(Action0::Bind<Lambda>(lambda));
	}

	template <class Lambda>
	ITimer* StartLambda(const TimeDuration& duration, Lambda& lambda)
	{
		return this->Start(duration, Action0::Bind<Lambda>(lambda));
	}
};

}

#endif
