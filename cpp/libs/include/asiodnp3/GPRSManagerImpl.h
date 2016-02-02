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
#ifndef ASIODNP3_GPRSMANAGERIMPL_H
#define ASIODNP3_GPRSMANAGERIMPL_H

#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/LogFanoutHandler.h>
#include <asiopal/IListener.h>

#include <openpal/util/Uncopyable.h>
#include <openpal/logging/LogRoot.h>

#include <asiodnp3/MasterTCPServer.h>

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

namespace asiodnp3
{

class GPRSManagerImpl : private openpal::Uncopyable
{
	
public:	
					
	~GPRSManagerImpl();

	void BeginShutdown();

	std::shared_ptr<asiopal::IListener> CreateListener(
		std::string loggerid,
		openpal::LogFilters loglevel,
		asiopal::IPEndpoint endpoint,
		std::error_code& ec
	);

private:

	friend class GPRSManager;

	GPRSManagerImpl(uint32_t concurrencyHint, openpal::ILogHandler& handler);	

	std::mutex m_mutex;

	openpal::ILogHandler* m_log_handler;
	openpal::LogRoot m_log_root;
	bool m_is_shutting_down;
	openpal::ILogHandler* m_log;
	asiopal::IOServiceThreadPool m_pool;

	std::vector<std::shared_ptr<MasterTCPServer>> m_servers;
	
};

}

#endif
