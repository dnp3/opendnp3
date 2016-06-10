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

#include "asiopal/StrandExecutor.h"
#include "asiopal/StrandTimer.h"

#include <chrono>

using namespace openpal;

namespace asiopal
{

StrandExecutor::StrandExecutor(std::shared_ptr<ThreadPool> pool) :
	m_pool(pool),
	m_strand(pool->GetIOService())
{

}

std::shared_ptr<StrandExecutor> StrandExecutor::Create(std::shared_ptr<ThreadPool> pool)
{
	return std::shared_ptr<StrandExecutor>(new StrandExecutor(pool));
}

MonotonicTimestamp StrandExecutor::GetTime()
{
	auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(asiopal_steady_clock::now().time_since_epoch()).count();
	return openpal::MonotonicTimestamp(millisec);
}

ITimer* StrandExecutor::Start(const TimeDuration& delay, const Action0& runnable)
{
	auto expiration = asiopal_steady_clock::now() + std::chrono::milliseconds(delay.GetMilliseconds());
	return Start(expiration, runnable);
}

ITimer* StrandExecutor::Start(const MonotonicTimestamp& time, const Action0& runnable)
{
	asiopal_steady_clock::time_point expiration(std::chrono::milliseconds(time.milliseconds));
	return Start(expiration, runnable);
}

openpal::ITimer* StrandExecutor::Start(const asiopal_steady_clock::time_point& expiration, const openpal::Action0& runnable)
{
	auto self(shared_from_this());
	auto timer = std::shared_ptr<StrandTimer>(new StrandTimer(this->m_strand.get_io_service()));

	timer->m_timer.expires_at(expiration);

	// neither the executor nor the timer can be deleted while the timer is still active
	auto callback = [timer, self, runnable](const std::error_code & ec)
	{
		if (!ec)   // an error indicate timer was canceled
		{
			runnable.Apply();
		}
	};

	timer->m_timer.async_wait(m_strand.wrap(callback));

	return timer.get();
}

void StrandExecutor::Post(const Action0& runnable)
{
	auto self(shared_from_this());
	auto callback = [self, runnable]()
	{
		runnable.Apply();
	};
	m_strand.post(callback);
}

}
