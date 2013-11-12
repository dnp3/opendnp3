
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

#include "PhysicalLayerAsyncTCPClient.h"

#include <boost/asio.hpp>
#include <functional>
#include <string>


#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>

using namespace boost;
using namespace boost::asio;
using namespace std;
using namespace openpal;

namespace opendnp3
{

PhysicalLayerAsyncTCPClient::PhysicalLayerAsyncTCPClient(Logger aLogger, boost::asio::io_service* apIOService, const std::string& arAddress, uint16_t aPort) :
	PhysicalLayerAsyncBaseTCP(aLogger, apIOService),
	mRemoteEndpoint(ip::tcp::v4(), aPort)
{
	mRemoteEndpoint.address( boost::asio::ip::address::from_string(arAddress) );
}

/* Implement the actions */
void PhysicalLayerAsyncTCPClient::DoOpen()
{
	mSocket.async_connect(mRemoteEndpoint,
	                      mStrand.wrap(
	                              std::bind(&PhysicalLayerAsyncTCPClient::OnOpenCallback,
	                                        this,
	                                        std::placeholders::_1)
	                      ));
}

void PhysicalLayerAsyncTCPClient::DoOpeningClose()
{
	this->CloseSocket();
}

void PhysicalLayerAsyncTCPClient::DoOpenSuccess()
{
	LOG_BLOCK(LEV_INFO, "Connected to: " << mRemoteEndpoint);
}

}

/* vim: set ts=4 sw=4: */
