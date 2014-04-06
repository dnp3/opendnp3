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
#include "PhysicalLayerAsyncSerial.h"

#include <asio.hpp>

#include <functional>
#include <string>

#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>
#include <openpal/LogLevels.h>

#include "ASIOSerialHelpers.h"

using namespace asio;
using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncSerial::PhysicalLayerAsyncSerial(
    const openpal::LogConfig& config,
    asio::io_service* apIOService,
    const SerialSettings& settings) :

	PhysicalLayerAsyncASIO(config, apIOService),
	mSettings(settings),
	mPort(*apIOService)
{

}

/* Implement the actions */

void PhysicalLayerAsyncSerial::DoOpen()
{
	std::error_code ec;
	mPort.open(mSettings.mDevice, ec);

	if (!ec)
	{
		Configure(mSettings, mPort, ec);
		if(ec)
		{
			std::error_code ec2;
			mPort.close(ec2);
		}
	}

	//use post to simulate an async open operation
	auto lambda = [this, ec]() { this->OnOpenCallback(ec); };
	executor.PostLambda(lambda);
}

void PhysicalLayerAsyncSerial::DoClose()
{
	std::error_code ec;
	mPort.close(ec);
	if (ec)
	{
		//LOG_BLOCK(logflags::WARN, ec.message());
	}
}

void PhysicalLayerAsyncSerial::DoOpenSuccess()
{
	LOG_BLOCK(logflags::INFO, "Port successfully opened");
}

void PhysicalLayerAsyncSerial::DoAsyncRead(openpal::WriteBuffer& buff)
{
	uint8_t* pBuffer = buff;
	mPort.async_read_some(buffer(pBuffer, buff.Size()),
	                      strand.wrap(
	                          [this, pBuffer](const std::error_code & error, size_t numRead)
	{
		this->OnReadCallback(error, pBuffer, static_cast<uint32_t>(numRead));
	}
	                      )
	                     );
}

void PhysicalLayerAsyncSerial::DoAsyncWrite(const ReadOnlyBuffer& buff)
{
	async_write(mPort, buffer(buff, buff.Size()),
	            strand.wrap(
	                std::bind(&PhysicalLayerAsyncSerial::OnWriteCallback,
	                          this,
	                          std::placeholders::_1,
	                          buff.Size())
	            ));
}

}



