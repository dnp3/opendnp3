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

#include "asiodnp3/tls/MasterTLSServer.h"

#include "asiodnp3/LinkSession.h"

#include "asiopal/tls/TLSStreamChannel.h"

#include <openpal/logging/LogMacros.h>
#include <opendnp3/LogLevels.h>

using namespace openpal;
using namespace asiopal;
using namespace opendnp3;

namespace asiodnp3
{

std::shared_ptr<MasterTLSServer> MasterTLSServer::Create(
    IResourceManager& shutdown,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::shared_ptr<asiopal::ThreadPool> pool,
    LogRoot root,
    IPEndpoint endpoint,
    const TLSConfig& config,
    std::error_code& ec)
{
	auto ret = std::shared_ptr<MasterTLSServer>(new MasterTLSServer(shutdown, callbacks, pool, std::move(root), endpoint, config, ec));
	if (!ec)
	{
		ret->StartAccept(ec);
	}
	return ret;
}

MasterTLSServer::MasterTLSServer(
    IResourceManager& shutdown,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::shared_ptr<asiopal::ThreadPool> pool,
    LogRoot root,
    IPEndpoint endpoint,
    const TLSConfig& config,
    std::error_code& ec) :
	TLSServer(pool, std::move(root), endpoint, config, ec),
	m_manager(&shutdown),
	m_callbacks(callbacks)
{

}

void MasterTLSServer::OnShutdown()
{
	m_manager->Unregister(shared_from_this());
}


bool MasterTLSServer::AcceptConnection(uint64_t sessionid, const asio::ip::tcp::endpoint& remote)
{
	std::ostringstream oss;
	oss << remote;

	if (m_callbacks->AcceptConnection(sessionid, remote.address().to_string()))
	{
		FORMAT_LOG_BLOCK(m_root.logger, flags::INFO, "Accepted connection from: %s", oss.str().c_str());
		return true;
	}
	else
	{
		FORMAT_LOG_BLOCK(m_root.logger, flags::INFO, "Rejected connection from: %s", oss.str().c_str());
		return false;
	}
}

bool MasterTLSServer::VerifyCallback(uint64_t sessionid, bool preverified, asio::ssl::verify_context& ctx)
{
	int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());

	if (!preverified)
	{
		FORMAT_LOG_BLOCK(this->m_root.logger, flags::WARN, "Error verifying certificate at depth: %d", depth);
		return preverified;
	}

	// lookup the subject name
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	char subjectName[512];
	X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 512);

	FORMAT_LOG_BLOCK(this->m_root.logger, flags::INFO, "Depth: %d - Verified certificate: %s", depth, subjectName);

	return this->m_callbacks->AcceptCertificate(
	           sessionid,
	           X509Info(
	               depth,
	               RSlice(cert->sha1_hash, SHA_DIGEST_LENGTH), // the thumbprint
	               std::string(subjectName)
	           )
	       );
}

void MasterTLSServer::AcceptStream(uint64_t sessionid, std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream)
{
	LinkSession::Create(
	    m_root.Clone(SessionIdToString(sessionid).c_str()),
	    sessionid,
	    *m_manager,
	    m_callbacks,
	    StrandExecutor::Create(m_pool),
	    TLSStreamChannel::Create(stream)
	);
}

std::string MasterTLSServer::SessionIdToString(uint64_t sessionid)
{
	std::ostringstream oss;
	oss << "session-" << sessionid;
	return oss.str();
}

}

