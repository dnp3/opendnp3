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

#include "asiodnp3/SocketSession.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

#include <sstream>

using namespace opendnp3;
using namespace openpal;
using namespace asiopal;

namespace asiodnp3
{			

std::shared_ptr<MasterTCPServer> MasterTCPServer::Create(
	asio::io_service& ioservice, 
	IResourceManager& shutdown,
	std::shared_ptr<IListenCallbacks> callbacks,
	openpal::LogRoot root,
	asiopal::IPEndpoint endpoint,
	std::error_code& ec
)
{
	auto ret = std::shared_ptr<MasterTCPServer>(new MasterTCPServer(ioservice, shutdown, callbacks, std::move(root), endpoint, ec));
	ret->StartAccept();
	return ret;
}

MasterTCPServer::MasterTCPServer(
		asio::io_service& ioservice,
		IResourceManager& shutdown,
		std::shared_ptr<IListenCallbacks> callbacks,
		openpal::LogRoot root,		
		asiopal::IPEndpoint endpoint, 
		std::error_code& ec
) :
	TCPServer(ioservice, std::move(root), endpoint, ec),
	m_manager(&shutdown),
	m_callbacks(callbacks),
	m_accept_count(0)
{

}
				
void MasterTCPServer::AcceptConnection(asio::ip::tcp::socket socket)
{
	std::ostringstream oss;
	oss << socket.remote_endpoint();

	const auto SESSION_ID = m_accept_count;
	++m_accept_count;
	
	if (m_callbacks->AcceptConnection(SESSION_ID, socket.remote_endpoint().address().to_string()))
	{
		FORMAT_LOG_BLOCK(m_root.logger, flags::INFO, "Accepted connection from: %s", oss.str().c_str());				

		SocketSession::Create(m_root.Clone(SessionIdToString(SESSION_ID).c_str()), SESSION_ID, *m_manager, *m_callbacks, std::move(socket));
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


