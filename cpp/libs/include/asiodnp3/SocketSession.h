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

#ifndef ASIODNP3_SOCKETSESSION_H
#define ASIODNP3_SOCKETSESSION_H

#include <openpal/logging/LogRoot.h>
#include <openpal/executor/TimerRef.h>

#include <opendnp3/link/LinkLayerParser.h>
#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/LinkChannelStatistics.h>
#include <opendnp3/Route.h>

#include <asiopal/StrandExecutor.h>
#include <asiopal/IResourceManager.h>

#include "asiodnp3/GPRSMasterStack.h"
#include "asiodnp3/IListenCallbacks.h"

namespace asiodnp3
{		
	class SocketSession final : 
		public opendnp3::ILinkTx,
		private opendnp3::IFrameSink,
		public std::enable_shared_from_this<SocketSession>,
		public asiopal::IResource,
		private ISessionAcceptor,
		private openpal::Uncopyable
	{
	public:		

		static std::shared_ptr<SocketSession> Create(
			openpal::LogRoot logroot,
			asiopal::IResourceManager& manager,
			IListenCallbacks& callbacks,
			asio::ip::tcp::socket socket
		);
		
		// override IResource
		void BeginShutdown() override;

	private:		

		// ILinkTx
		virtual void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& session) override;

		// IFrameSink
		virtual bool OnFrame(const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata) override;

		// ISessionAcceptor
		virtual std::shared_ptr<IGPRSMaster> AcceptSession(
			const std::string& loggerid,
			opendnp3::ISOEHandler& SOEHandler,
			opendnp3::IMasterApplication& application,
			const opendnp3::MasterStackConfig& config) override;

		void Start();		

		void BeginReceive();		

		SocketSession(
			openpal::LogRoot logroot,
			asiopal::IResourceManager& manager,
			IListenCallbacks& callbacks,
			asio::ip::tcp::socket socket
		);

		openpal::LogRoot m_log_root;

		asiopal::IResourceManager* m_manager;
		IListenCallbacks* m_callbacks;
		opendnp3::LinkChannelStatistics m_stats;
		opendnp3::LinkLayerParser m_parser;		
		std::shared_ptr<asiopal::StrandExecutor> m_executor;
		openpal::TimerRef m_first_frame_timer;
		opendnp3::Route m_route;
		

		// this will become some kind of shared ptr to an interface
		// so that the same class can handle TCP or TLS
		asio::ip::tcp::socket m_socket;
		std::shared_ptr<GPRSMasterStack> m_stack;	// initialized to null
	};
}

#endif

