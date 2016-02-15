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
#ifndef ASIODNP3_ILISTENCALLBACKS_H
#define ASIODNP3_ILISTENCALLBACKS_H

#include <openpal/executor/TimeDuration.h>
#include <opendnp3/link/LinkHeaderFields.h>

#include "asiodnp3/ISessionAcceptor.h"

namespace asiodnp3
{

/**
* Callback interface invoked when a new connection is accepted
*/
class IListenCallbacks
{
public:

	virtual ~IListenCallbacks() {}

	/**
	* Ask user code if the following connection should be accepted
	*
	* @ipaddress The IP address of the connecting host. Can optionally be used for connection filtering.	
	*
	* @return If true, the connection is accepted and a link frame parser is created to handle incoming frame data.
	*/
	virtual bool AcceptConnection(const std::string& ipaddress) = 0;

	/// return the amount of time the session should wait for the first frame
	virtual openpal::TimeDuration GetFirstFrameTimeout() = 0;

	/// called when the first link-layer frame is received
	virtual void OnFirstFrame(
		const opendnp3::LinkHeaderFields& header,		
		ISessionAcceptor& acceptor
	) = 0;

	/// Called when an active master session closes
	virtual void OnSessionClose(std::shared_ptr<IMasterSession> session, const std::string& sessionid) = 0;
};

}

#endif
