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
#ifndef ASIOPAL_PHYSICAL_LAYER_BASE_TCP_H
#define ASIOPAL_PHYSICAL_LAYER_BASE_TCP_H

#include "PhysicalLayerASIO.h"

#include <asio.hpp>
#include <asio/ip/tcp.hpp>

#include <memory>

namespace asiopal
{

/**
Common socket object and some shared implementations for server/client.
*/
class PhysicalLayerBaseTCP : public PhysicalLayerASIO
{
public:
	PhysicalLayerBaseTCP(openpal::Logger logger, asio::io_service& service);

	virtual ~PhysicalLayerBaseTCP() {}

	/* Implement the shared client/server actions */
	void DoClose();
	void DoRead(openpal::WSlice&);
	void DoWrite(const openpal::RSlice&);
	void DoOpenFailure();

protected:

	asio::ip::tcp::socket socket;
	void CloseSocket();

private:
	void ShutdownSocket();

};
}

#endif
