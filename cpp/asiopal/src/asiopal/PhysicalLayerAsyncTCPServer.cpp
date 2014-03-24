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
#include "PhysicalLayerAsyncTCPServer.h"

#include <asio.hpp>

#include <functional>
#include <string>

#include <openpal/LoggableMacros.h>
#include <openpal/IHandlerAsync.h>
#include <openpal/LogLevels.h>

using namespace asio;
using namespace openpal;
using namespace std;

namespace asiopal
{

PhysicalLayerAsyncTCPServer::PhysicalLayerAsyncTCPServer(
    Logger aLogger,
    asio::io_service* apIOService,
    const std::string& arEndpoint,
    uint16_t aPort,
    std::function<void (asio::ip::tcp::socket&)> aConfigure) :

	PhysicalLayerAsyncBaseTCP(aLogger, apIOService),
	mLocalEndpoint(ip::tcp::v4(), aPort),
	mAcceptor(*apIOService),
	mConfigure(aConfigure)
{
	mLocalEndpoint.address( asio::ip::address::from_string(arEndpoint) );
}

/* Implement the actions */
void PhysicalLayerAsyncTCPServer::DoOpen()
{
	if (!mAcceptor.is_open())
	{
		std::error_code ec;
		mAcceptor.open(mLocalEndpoint.protocol(), ec);
		if (ec)
		{
			LOG_BLOCK(log::ERR, ec.message());
			this->GetExecutor()->Post([this, ec]()
			{
				this->OnOpenCallback(ec);
			});
		}
		else
		{
			mAcceptor.set_option(ip::tcp::acceptor::reuse_address(true));
			mAcceptor.bind(mLocalEndpoint, ec);
			if (ec)
			{
				LOG_BLOCK(log::ERR, ec.message());
				this->GetExecutor()->Post([this, ec]()
				{
					this->OnOpenCallback(ec);
				});
			}
			else
			{
				mAcceptor.listen(socket_base::max_connections, ec);
				if (ec)
				{
					LOG_BLOCK(log::ERR, ec.message());
					this->GetExecutor()->Post([this, ec]()
					{
						this->OnOpenCallback(ec);
					});
				}
				else
				{
					mAcceptor.async_accept(mSocket,
					                       mRemoteEndpoint,
					                       strand.wrap([this](const std::error_code & code)
					{
						this->OnOpenCallback(code);
					}));
				}
			}
		}
	}
	else
	{
		mAcceptor.async_accept(mSocket,
		                       mRemoteEndpoint,
		                       strand.wrap([this](const std::error_code & code)
		{
			this->OnOpenCallback(code);
		}));
	}
}

void PhysicalLayerAsyncTCPServer::CloseAcceptor()
{
	std::error_code ec;
	mAcceptor.close(ec);
	if(ec)
	{
		LOG_BLOCK(log::WARN, "Error while closing tcp acceptor: " << ec);
	}
}

void PhysicalLayerAsyncTCPServer::DoOpenCallback()
{
	this->CloseAcceptor();
}

void PhysicalLayerAsyncTCPServer::DoOpeningClose()
{
	this->CloseAcceptor();
}

void PhysicalLayerAsyncTCPServer::DoOpenSuccess()
{
	LOG_BLOCK(log::INFO, "Accepted connection from: " << mRemoteEndpoint);
	mConfigure(mSocket);
}

}


