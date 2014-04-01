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
#include "PhysicalLayerAsyncBaseTCP.h"

#include <string>
#include <functional>

#include <asio.hpp>
#include <asio/ip/tcp.hpp>

#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>
#include <openpal/LogLevels.h>

using namespace asio;

using namespace std;
using namespace openpal;

namespace asiopal
{

PhysicalLayerAsyncBaseTCP::PhysicalLayerAsyncBaseTCP(const openpal::LogConfig& config, asio::io_service* apIOService) :
	PhysicalLayerAsyncASIO(config, apIOService),
	mSocket(*apIOService)
{
	//mSocket.set_option(ip::tcp::no_delay(true));
}

/* Implement the actions */

void PhysicalLayerAsyncBaseTCP::DoClose()
{
	this->ShutdownSocket();
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseTCP::DoAsyncRead(WriteBuffer& buff)
{
	uint8_t* pBuff = buff;
	mSocket.async_read_some(buffer(pBuff, buff.Size()),
	                        strand.wrap([this, pBuff](const std::error_code & code, size_t  numRead)
	{
		this->OnReadCallback(code, pBuff, static_cast<uint32_t>(numRead));
	}));
}

void PhysicalLayerAsyncBaseTCP::DoAsyncWrite(const ReadOnlyBuffer& buff)
{
	async_write(mSocket, buffer(buff, buff.Size()),
	            strand.wrap([this](const std::error_code & code, size_t  numWritten)
	{
		this->OnWriteCallback(code, static_cast<uint32_t>(numWritten));
	}));
}

void PhysicalLayerAsyncBaseTCP::DoOpenFailure()
{
	LOG_BLOCK(log::DEBUG, "Failed socket open, closing socket");
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseTCP::CloseSocket()
{
	std::error_code ec;

	mSocket.close(ec);
	if (ec)
	{
		LOG_BLOCK(log::WARN, "Error while closing socket: " << ec.message());
	}
}

void PhysicalLayerAsyncBaseTCP::ShutdownSocket()
{
	std::error_code ec;

	mSocket.shutdown(ip::tcp::socket::shutdown_both, ec);
	if (ec)
	{
		LOG_BLOCK(log::WARN, "Error while shutting down socket: " << ec.message());
	}
}

}


