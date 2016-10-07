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

#include "asiopal/StrandExecutor.h"

#include <functional>
#include <system_error>
#include <memory>

namespace asiopal
{

typedef std::function<void (const std::error_code& ec, std::size_t num)> read_callback_t;
typedef std::function<void (const std::error_code& ec, std::size_t num)> write_callback_t;

class IAsyncChannel : public std::enable_shared_from_this<IAsyncChannel>, private openpal::Uncopyable
{
public:	

	IAsyncChannel(std::shared_ptr<StrandExecutor> executor) : executor(executor)
	{}
	
	virtual ~IAsyncChannel() {}

	template <class T>
	bool BeginRead(openpal::WSlice& buffer, const T& callback)
	{
		if (this->reading || this->shuttingDown) return false;
		this->reading = true;

		std::shared_ptr<IAsyncChannel> self(this->shared_from_this());
		
		auto cbwrap = [self, buffer, callback](const std::error_code& ec, std::size_t num)
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

	template <class T>
	bool BeginWrite(const openpal::RSlice& buffer, const T& callback)
	{
		if (this->writing || this->shuttingDown) return false;
		this->writing = true;
		
		std::shared_ptr<IAsyncChannel> self(this->shared_from_this());

		auto cbwrap = [self, buffer, callback](const std::error_code& ec, std::size_t num)
		{
			self->writing = false;
			if (!self->shuttingDown)
			{
				callback(ec, num);
			}

		};
		
		this->BeginWriteImpl(buffer, this->executor->strand.wrap(cbwrap));
		return true;
	}
		
	bool BeginShutdown()
	{
		if (this->shuttingDown) return false;
		this->shuttingDown = true;
		this->BeginShutdownImpl();
		return true;
	}

	bool CanRead() const 
	{
		return !(reading || shuttingDown);
	}

	bool CanWrite() const
	{
		return !(writing || shuttingDown);
	}

	std::shared_ptr<StrandExecutor> executor;

protected:

	bool reading = false;
	bool writing = false;
	bool shuttingDown = false;

	virtual void BeginReadImpl(openpal::WSlice& buffer, const read_callback_t& callback) = 0;
	
	virtual void BeginWriteImpl(const openpal::RSlice& buffer, const write_callback_t& callback) = 0;
	
	virtual void BeginShutdownImpl() = 0;

};

}

#endif
