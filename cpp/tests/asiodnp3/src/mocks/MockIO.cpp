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

#include "MockIO.h"

#include <sstream>

namespace asiopal
{

size_t MockIO::RunUntilTimeout(const std::function<bool()>& condition, std::chrono::steady_clock::duration timeout)
{
	size_t iterations = 0;
	const auto start = std::chrono::steady_clock::now();

	while (!condition())
	{
		std::error_code ec;
		const auto num = this->service.poll_one(ec);
		if (ec) throw std::logic_error(ec.message());
		if (num == 0)
		{
			std::ostringstream oss;
			oss << "no progress after " << iterations << " iterations";
			throw std::logic_error(oss.str());
		}

		++iterations;

		this->service.reset();

		const auto now = std::chrono::steady_clock::now();

		if ((now - start) > timeout)
		{
			std::ostringstream oss;
			oss << "timeout while waiting for condition after " << iterations << " iterations";
			throw std::logic_error(oss.str());
		}
	}

	return iterations;
}

void MockIO::CompleteInXIterations(size_t expectedIterations, const std::function<bool()>& condition)
{
	size_t iterations = 0;

	while (!condition())
	{
		if (iterations == expectedIterations)
		{
			std::ostringstream oss;
			oss << "not complete after " << iterations << " iterations";
			throw std::logic_error(oss.str());
		}

		std::error_code ec;
		const auto num = this->service.poll_one(ec);
		if (ec) throw std::logic_error(ec.message());
		if (num == 0)
		{
			std::ostringstream oss;
			oss << "no progress after " << iterations << " iterations";
			throw std::logic_error(oss.str());
		}

		++iterations;
		this->service.reset();
	}

	if (iterations != expectedIterations)
	{
		std::ostringstream oss;
		oss << "completed after " << iterations << " iterations, (expected " << expectedIterations << ")";
		throw std::logic_error(oss.str());
	}
}

size_t MockIO::RunUntilOutOfWork()
{
	size_t iterations = 0;

	while (true)
	{
		std::error_code ec;
		const auto num = this->service.poll_one(ec);
		if (ec) throw std::logic_error(ec.message());

		if (num == 0)
		{
			return iterations;
		}

		++iterations;

		this->service.reset();
	}
}


}









