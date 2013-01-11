//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//

#include "PhysicalLayerAsyncBaseTCP.h"

#include <string>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Exception.h"
#include "IHandlerAsync.h"
#include "Logger.h"
#include "LoggableMacros.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace std;

namespace apl
{

PhysicalLayerAsyncBaseTCP::PhysicalLayerAsyncBaseTCP(Logger* apLogger, boost::asio::io_service* apIOService) :
	PhysicalLayerAsyncASIO(apLogger, apIOService),
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

void PhysicalLayerAsyncBaseTCP::DoAsyncRead(uint8_t* apBuffer, size_t aMaxBytes)
{
	mSocket.async_read_some(buffer(apBuffer, aMaxBytes),
	                        std::bind(&PhysicalLayerAsyncBaseTCP::OnReadCallback,
	                                    this,
										std::placeholders::_1,
	                                    apBuffer,
										std::placeholders::_2));
}

void PhysicalLayerAsyncBaseTCP::DoAsyncWrite(const uint8_t* apBuffer, size_t aNumBytes)
{
	async_write(mSocket, buffer(apBuffer, aNumBytes),
	            std::bind(&PhysicalLayerAsyncBaseTCP::OnWriteCallback,
	                        this,
							std::placeholders::_1,
	                        aNumBytes));
}

void PhysicalLayerAsyncBaseTCP::DoOpenFailure()
{
	LOG_BLOCK(LEV_DEBUG, "Failed socket open, closing socket");
	this->CloseSocket();
}

void PhysicalLayerAsyncBaseTCP::CloseSocket()
{
	boost::system::error_code ec;

	mSocket.close(ec);
	if(ec) LOG_BLOCK(LEV_WARNING, "Error while closing socket: " << ec.message());
}

void PhysicalLayerAsyncBaseTCP::ShutdownSocket()
{
	boost::system::error_code ec;

	mSocket.shutdown(ip::tcp::socket::shutdown_both, ec);
	if(ec) LOG_BLOCK(LEV_WARNING, "Error while shutting down socket: " << ec.message());
}

boost::asio::ip::address PhysicalLayerAsyncBaseTCP::ResolveAddress(const std::string& arEndpoint)
{
	try {
		boost::system::error_code ec;
		boost::asio::ip::address addr = boost::asio::ip::address::from_string(arEndpoint, ec);
		if (ec)
			throw ArgumentException(LOCATION, "endpoint: " + arEndpoint + " is invalid");
		return addr;
	} catch (...) {
		boost::asio::io_service                  io_service;
		boost::asio::ip::tcp::resolver           resolver(io_service);
		boost::asio::ip::tcp::resolver::query    query(arEndpoint, "");
		boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;
		while (iter != end)
		{
			boost::asio::ip::tcp::endpoint ep = *iter++;
			return ep.address();
		}
		throw ArgumentException(LOCATION, "endpoint: " + arEndpoint + " is invalid");
	}
}

}

/* vim: set ts=4 sw=4: */
