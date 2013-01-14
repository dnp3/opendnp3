
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "PhysicalLayerFactory.h"

#include "PhysicalLayerAsyncSerial.h"
#include "PhysicalLayerAsyncTCPClient.h"
#include "PhysicalLayerAsyncTCPServer.h"

#include "Log.h"

#include <boost/asio.hpp>
#include <functional>

using namespace std::placeholders;

namespace apl
{

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetSerialAsync(SerialSettings s)
{
	return std::bind(&PhysicalLayerFactory::FGetSerialAsync, s, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetTCPClientAsync(std::string aAddress, uint16_t aPort)
{
	return std::bind(&PhysicalLayerFactory::FGetTCPClientAsync, aAddress, aPort, _2, _1);
}

IPhysicalLayerAsyncFactory PhysicalLayerFactory :: GetTCPServerAsync(std::string aEndpoint, uint16_t aPort)
{
	return std::bind(&PhysicalLayerFactory::FGetTCPServerAsync, aEndpoint, aPort, _2, _1);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetSerialAsync(SerialSettings s, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncSerial(apLogger, apSrv, s);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetTCPClientAsync(std::string aAddress, uint16_t aPort, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncTCPClient(apLogger, apSrv, aAddress, aPort);
}

IPhysicalLayerAsync* PhysicalLayerFactory :: FGetTCPServerAsync(std::string aEndpoint, uint16_t aPort, boost::asio::io_service* apSrv, Logger* apLogger)
{
	return new PhysicalLayerAsyncTCPServer(apLogger, apSrv, aEndpoint, aPort);
}

}
