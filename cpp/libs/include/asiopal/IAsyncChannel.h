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
#ifndef ASIOPAL_IASYNCCHANNEL_H
#define ASIOPAL_IASYNCCHANNEL_H

#include <openpal/container/WSlice.h>
#include <openpal/container/RSlice.h>
#include <openpal/util/Uncopyable.h>

#include "asiopal/Executor.h"

#include <functional>
#include <system_error>
#include <memory>

namespace asiopal
{


class IAsyncChannel : public std::enable_shared_from_this<IAsyncChannel>, private openpal::Uncopyable
{
public:

	IAsyncChannel(std::shared_ptr<Executor> executor) : executor(executor)
	{}

	virtual ~IAsyncChannel() {}

	template <class IOCallback>
	inline bool BeginRead(const openpal::WSlice& buffer, const IOCallback& callback);

	template <class IOCallback>
	inline bool BeginWrite(const openpal::RSlice& buffer, const IOCallback& callback);

	inline bool Shutdown()
	{
		if (this->shuttingDown) return false;
		this->shuttingDown = true;

		this->ShutdownImpl();
		return true;
	}

	inline bool CanRead() const
	{
		return !(reading || shuttingDown);
	}


	inline bool CanWrite() const
	{
		return !(writing || shuttingDown);
	}

	std::shared_ptr<Executor> executor;

protected:

	bool reading = false;
	bool writing = false;
	bool shuttingDown = false;

	typedef std::function<void(const std::error_code& ec, std::size_t num)> io_callback_t;

	virtual void BeginReadImpl(openpal::WSlice buffer, const io_callback_t& callback) = 0;
	virtual void BeginWriteImpl(const openpal::RSlice& buffer, const io_callback_t& callback) = 0;
	virtual void ShutdownImpl() = 0;
};

template <class IOCallback>
bool IAsyncChannel::BeginRead(const openpal::WSlice& buffer, const IOCallback& callback)
{
	if (!this->CanRead()) return false;

	this->reading = true;

	auto cbwrap = [self = shared_from_this(), buffer, callback](const std::error_code & ec, std::size_t num)
	{
		self->reading = false;
		if (!self->shuttingDown)
		{
			callback(ec, num);
		}
	};

	this->BeginReadImpl(buffer, this->executor->strand.wrap(cbwrap));

	return true;
}

template <class IOCallback>
bool IAsyncChannel::BeginWrite(const openpal::RSlice& buffer, const IOCallback& callback)
{
	if (!this->CanWrite()) return false;

	this->writing = true;

	auto cbwrap = [self = shared_from_this(), buffer, callback](const std::error_code & ec, std::size_t num)
	{
		self->writing = false;
		if (!self->shuttingDown)
		{
			callback(ec);
		}
	};

	this->BeginWriteImpl(buffer, this->executor->strand.wrap(cbwrap));
	return true;
}

}

#endif
