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

#include "asiopal/SerialChannel.h"

#include "asiopal/ASIOSerialHelpers.h"


namespace asiopal
{


SerialChannel::SerialChannel(asio::io_service& service) : port(service)
{}

std::unique_ptr<SerialChannel> SerialChannel::Create(asio::io_service& service)
{
	return std::make_unique<SerialChannel>(service);
}

void SerialChannel::Open(const SerialSettings& settings, std::error_code& ec)
{
	port.open(settings.deviceName, ec);
	if (ec) return;

	Configure(settings, port, ec);
	
	if (ec)
	{
		port.close();	
	}	
}

void SerialChannel::BeginRead(openpal::WSlice& buffer, const read_callback_t& callback)
{
	port.async_read_some(asio::buffer(buffer, buffer.Size()), callback);
}

void SerialChannel::BeginWrite(const openpal::RSlice& buffer, const write_callback_t& callback)
{
	async_write(port, asio::buffer(buffer, buffer.Size()), callback);
}

void SerialChannel::BeginShutdown(const shutdown_callback_t& callback)
{
	std::error_code ec;
	port.close(ec);
	callback(ec);
}

}


