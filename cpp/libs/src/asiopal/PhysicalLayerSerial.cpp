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
#include "asiopal/PhysicalLayerSerial.h"

#include <asio.hpp>

#include <functional>
#include <string>

#include <openpal/logging/LogMacros.h>
#include <openpal/channel/IPhysicalLayerCallbacks.h>
#include <openpal/logging/LogLevels.h>

#include "asiopal/ASIOSerialHelpers.h"

using namespace asio;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerSerial::PhysicalLayerSerial(
    openpal::Logger logger,
    asio::io_service& service,
    const SerialSettings& settings) :

	PhysicalLayerASIO(logger, service),
	settings(settings),
	port(service)
{

}

/* Implement the actions */

void PhysicalLayerSerial::DoOpen()
{
	std::error_code ec;
	port.open(settings.deviceName, ec);

	if (!ec)
	{
		Configure(settings, port, ec);
		if(ec)
		{
			std::error_code ec2;
			port.close(ec2);
		}
	}

	auto lambda = [this, ec]()
	{
		this->OnOpenCallback(ec);
	};

	if (settings.asyncOpenDelay.GetMilliseconds() > 0)
	{
		executor.Start(settings.asyncOpenDelay, Action0::Bind(lambda));
	}
	else
	{
		executor.PostLambda(lambda);
	}
}

void PhysicalLayerSerial::DoClose()
{
	std::error_code ec;
	port.close(ec);
	if (ec)
	{
		SIMPLE_LOG_BLOCK(logger, logflags::WARN, ec.message().c_str());
	}
}

void PhysicalLayerSerial::DoOpenSuccess()
{

}

void PhysicalLayerSerial::DoRead(openpal::WSlice& buff)
{
	uint8_t* pBuffer = buff;

	auto callback = [this, pBuffer](const std::error_code & error, size_t numRead)
	{
		this->OnReadCallback(error, pBuffer, static_cast<uint32_t>(numRead));
	};

	port.async_read_some(buffer(pBuffer, buff.Size()), executor.strand.wrap(callback));
}

void PhysicalLayerSerial::DoWrite(const RSlice& buff)
{
	auto callback = [this](const std::error_code & error, size_t size)
	{
		this->OnWriteCallback(error, static_cast<uint32_t>(size));
	};

	async_write(port, buffer(buff, buff.Size()), executor.strand.wrap(callback));
}

}



