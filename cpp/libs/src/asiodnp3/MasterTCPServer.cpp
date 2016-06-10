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

#include "asiodnp3/MasterTCPServer.h"

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

std::shared_ptr<MasterTCPServer> MasterTCPServer::Create(
    IResourceManager& shutdown,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::shared_ptr<ThreadPool> pool,
    openpal::LogRoot root,
    asiopal::IPEndpoint endpoint,
    std::error_code& ec
)
{
	auto ret = std::shared_ptr<MasterTCPServer>(new MasterTCPServer(shutdown, callbacks, pool, std::move(root), endpoint, ec));
	if (!ec)
	{
		ret->StartAccept();
	}
	return ret;
}

MasterTCPServer::MasterTCPServer(
    IResourceManager& shutdown,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::shared_ptr<ThreadPool> pool,
    openpal::LogRoot root,
    asiopal::IPEndpoint endpoint,
    std::error_code& ec
) :
	TCPServer(pool, std::move(root), endpoint, ec),
	m_manager(&shutdown),
	m_callbacks(callbacks)
{

}

void MasterTCPServer::AcceptConnection(uint64_t sessionid, asio::ip::tcp::socket socket)
{
	std::ostringstream oss;
	oss << socket.remote_endpoint();

	if (m_callbacks->AcceptConnection(sessionid, socket.remote_endpoint().address().to_string()))
	{
		FORMAT_LOG_BLOCK(m_root.logger, flags::INFO, "Accepted connection from: %s", oss.str().c_str());

		LinkSession::Create(
		    m_root.Clone(SessionIdToString(sessionid).c_str()),
		    sessionid,
		    *m_manager,
		    m_callbacks,
		    StrandExecutor::Create(m_pool),
		    SocketChannel::Create(std::move(socket))
		);
	}
	else
	{
		socket.close();
		FORMAT_LOG_BLOCK(m_root.logger, flags::INFO, "Rejected connection from: %s", oss.str().c_str());
	}
}

std::string MasterTCPServer::SessionIdToString(uint64_t sessionid)
{
	std::ostringstream oss;
	oss << "session-" << sessionid;
	return oss.str();
}

void MasterTCPServer::OnShutdown()
{
	m_manager->Unregister(shared_from_this());
}

}


