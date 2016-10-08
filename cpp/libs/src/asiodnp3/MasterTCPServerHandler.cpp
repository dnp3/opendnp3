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

#include "asiodnp3/MasterTCPServerHandler.h"

#include "asiodnp3/LinkSession.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include "asiopal/SocketChannel.h"

#include <sstream>

using namespace opendnp3;
using namespace openpal;
using namespace asiopal;

namespace asiodnp3
{

MasterTCPServerHandler::MasterTCPServerHandler(
    openpal::LogRoot root,
    std::shared_ptr<IListenCallbacks> callbacks,
    asiopal::IResourceManager& manager
) :
	root(std::move(root)),
	callbacks(callbacks),
	manager(manager)
{

}

void MasterTCPServerHandler::AcceptConnection(uint64_t sessionid, const std::shared_ptr<asiopal::StrandExecutor>& executor, asio::ip::tcp::socket socket)
{
	std::ostringstream oss;
	oss << socket.remote_endpoint();

	if (this->callbacks->AcceptConnection(sessionid, socket.remote_endpoint().address().to_string()))
	{
		FORMAT_LOG_BLOCK(this->root.logger, flags::INFO, "Accepted connection from: %s", oss.str().c_str());

		auto session = LinkSession::Create(
		                   root.Clone(SessionIdToString(sessionid).c_str()),
		                   sessionid,
		                   this->callbacks,
		                   SocketChannel::Create(executor->Fork(), std::move(socket))	// run the link session in its own strand
		               );

		if (this->manager.Attach(session))
		{
			auto pmanager = &this->manager;
			session->SetShutdownAction([session, pmanager]()
			{
				pmanager->Detach(session);
			});
		}
		else
		{
			session->BeginShutdown();
		}
	}
	else
	{
		socket.close();
		FORMAT_LOG_BLOCK(this->root.logger, flags::INFO, "Rejected connection from: %s", oss.str().c_str());
	}
}

std::string MasterTCPServerHandler::SessionIdToString(uint64_t sessionid)
{
	std::ostringstream oss;
	oss << "session-" << sessionid;
	return oss.str();
}

}


