
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include "PhysicalLayerAsyncBaseTCP.h"

#include <string>
#include <functional>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <opendnp3/Exception.h>

#include <openpal/LoggableMacros.h>

#include "IHandlerAsync.h"


using namespace boost;
using namespace boost::asio;
using namespace boost::system;
using namespace std;
using namespace openpal;

namespace opendnp3
{

PhysicalLayerAsyncBaseTCP::PhysicalLayerAsyncBaseTCP(Logger& arLogger, boost::asio::io_service* apIOService) :
	PhysicalLayerAsyncASIO(arLogger, apIOService),
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
	                        mStrand.wrap(
	                                std::bind(&PhysicalLayerAsyncBaseTCP::OnReadCallback,
	                                                this,
	                                                std::placeholders::_1,
	                                                apBuffer,
	                                                std::placeholders::_2)
	                        ));
}

void PhysicalLayerAsyncBaseTCP::DoAsyncWrite(const uint8_t* apBuffer, size_t aNumBytes)
{
	async_write(mSocket, buffer(apBuffer, aNumBytes),
	            mStrand.wrap(
	                    std::bind(&PhysicalLayerAsyncBaseTCP::OnWriteCallback,
	                              this,
	                              std::placeholders::_1,
	                              aNumBytes)
	            ));
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

}

/* vim: set ts=4 sw=4: */
