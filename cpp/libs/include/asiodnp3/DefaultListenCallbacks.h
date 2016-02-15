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
#ifndef ASIODNP3_DEFAULTLISTENCALLBACKS_H
#define ASIODNP3_DEFAULTLISTENCALLBACKS_H

#include "asiodnp3/IListenCallbacks.h"
#include "asiodnp3/PrintingSOEHandler.h"
#include "asiodnp3/DefaultMasterApplication.h"

#include <iostream>
#include <mutex>

namespace asiodnp3
{

/**
* Callback interface invoked when a new connection is accepted
*/
class DefaultListenCallbacks final : public IListenCallbacks
{
public:

	DefaultListenCallbacks();

	virtual ~DefaultListenCallbacks() {}
	
	virtual bool AcceptConnection(const std::string& ipaddress) override;

	virtual openpal::TimeDuration GetFirstFrameTimeout() override;

	virtual void OnFirstFrame(const opendnp3::LinkHeaderFields& header, ISessionAcceptor& acceptor) override;

	virtual void OnSessionClose(std::shared_ptr<IMasterSession> session, const std::string& sessionid) override;

private:	

	std::string GetNextSessionId();

	std::mutex m_mutex;
	std::uint64_t m_session_count;
};

}

#endif

