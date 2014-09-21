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
#ifndef ASIOPAL_PHYSICALLAYERTCPCLIENT_H
#define ASIOPAL_PHYSICALLAYERTCPCLIENT_H

#include "PhysicalLayerBaseTCP.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

namespace asiopal
{

class PhysicalLayerTCPClient : public PhysicalLayerBaseTCP
{
public:
	PhysicalLayerTCPClient(
		openpal::LogRoot& root,
		asio::io_service& service,
	    const std::string& host_,
	    uint16_t port,
	std::function<void (asio::ip::tcp::socket&)> configure = [](asio::ip::tcp::socket&) {});

	/* Implement the remaining actions */
	void DoOpen();
	void DoOpeningClose(); //override this to just close the socket insead of shutting is down too
	void DoOpenSuccess();

private:

	struct ConnectionCondition
	{
		ConnectionCondition(openpal::Logger logger_) : logger(logger_)
		{}

		template <typename Iterator>
		Iterator operator()(const std::error_code& ec, Iterator next)
		{
			if (ec)
			{
				FORMAT_LOG_BLOCK(logger, openpal::logflags::WARN, "connection error: %s", ec.message().c_str());
			}			
			return next;
		}

		openpal::Logger logger;
	};

	void HandleResolve(const std::error_code& code, asio::ip::tcp::resolver::iterator endpoint_iterator);

	ConnectionCondition condition;
	const std::string host;
	asio::ip::tcp::endpoint remoteEndpoint;
	asio::ip::tcp::resolver resolver;
	std::function<void (asio::ip::tcp::socket&)> configure;
};

}

#endif
