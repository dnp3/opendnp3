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
#ifndef __PHYSICAL_LAYER_ASYNC_SERIAL_H_
#define __PHYSICAL_LAYER_ASYNC_SERIAL_H_

#include "PhysicalLayerAsyncASIO.h"
#include "SerialTypes.h"
#include <boost/asio/serial_port.hpp>

#include <memory>

namespace apl
{

/** Serial implementation of PhysicalLayerAsyncASIO
*/
class PhysicalLayerAsyncSerial : public PhysicalLayerAsyncASIO
{
public:
	PhysicalLayerAsyncSerial(Logger*, boost::asio::io_service* apIOService, const SerialSettings& arSettings);

	/* Implement the shared client/server actions */
	void DoClose();
	void DoOpenSuccess();
	void DoAsyncRead(uint8_t*, size_t);
	void DoAsyncWrite(const uint8_t*, size_t);

	void DoOpen();

protected:

	SerialSettings mSettings;
	boost::asio::io_service* mpService;
	boost::asio::serial_port mPort;
};
}

#endif
