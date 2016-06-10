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

#ifndef ASIODNP3_LINKSESSION_H
#define ASIODNP3_LINKSESSION_H

#include <openpal/logging/LogRoot.h>
#include <openpal/executor/TimerRef.h>

#include <opendnp3/link/LinkLayerParser.h>
#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/LinkChannelStatistics.h>
#include <opendnp3/Route.h>

#include <asiopal/StrandExecutor.h>
#include <asiopal/IResourceManager.h>
#include <asiopal/IAsyncChannel.h>

#include "asiodnp3/MasterSessionStack.h"
#include "asiodnp3/IListenCallbacks.h"

namespace asiodnp3
{
class LinkSession final :
	public opendnp3::ILinkTx,
	private opendnp3::IFrameSink,
	public std::enable_shared_from_this<LinkSession>,
	public asiopal::IResource,
	private ISessionAcceptor,
	private openpal::Uncopyable
{
public:

	static std::shared_ptr<LinkSession> Create(
	    openpal::LogRoot logroot,
	    uint64_t sessionid,
	    asiopal::IResourceManager& manager,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::shared_ptr<asiopal::StrandExecutor> executor,
	    std::unique_ptr<asiopal::IAsyncChannel> channel
	);

	// override IResource
	void BeginShutdown() override;

	void SetLogFilters(openpal::LogFilters filters);

private:

	// ILinkTx
	virtual void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& session) override;

	// IFrameSink
	virtual bool OnFrame(const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata) override;

	// ISessionAcceptor
	virtual std::shared_ptr<IMasterSession> AcceptSession(
	    const std::string& loggerid,
	    std::shared_ptr<opendnp3::ISOEHandler> SOEHandler,
	    std::shared_ptr<opendnp3::IMasterApplication> application,
	    const opendnp3::MasterStackConfig& config) override;

	void Start();

	void BeginReceive();

	LinkSession(
	    openpal::LogRoot logroot,
	    uint64_t sessionid,
	    asiopal::IResourceManager& manager,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::shared_ptr<asiopal::StrandExecutor> executor,
	    std::unique_ptr<asiopal::IAsyncChannel> channel
	);

	openpal::LogRoot m_log_root;
	const uint64_t m_session_id;

	asiopal::IResourceManager* m_manager;
	std::shared_ptr<IListenCallbacks> m_callbacks;
	opendnp3::LinkChannelStatistics m_stats;
	opendnp3::LinkLayerParser m_parser;
	std::shared_ptr<asiopal::StrandExecutor> m_executor;
	openpal::TimerRef m_first_frame_timer;
	opendnp3::Route m_route;


	std::unique_ptr<asiopal::IAsyncChannel> m_channel;
	std::shared_ptr<MasterSessionStack> m_stack;	// initialized to null
};
}

#endif

