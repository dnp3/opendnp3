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
#ifndef ASIODNP3_ILISTENCALLBACK_H
#define ASIODNP3_ILISTENCALLBACK_H

#include <opendnp3/link/LinkHeaderFields.h>
#include <opendnp3/master/MasterStackConfig.h>

namespace asiodnp3
{

/// Configuration information that will be filled in by the user code.
class MasterListenConfig
{
	public:

	MasterListenConfig() :
		config(),
		loggerId("master"),
		SOEHandler(nullptr),
		application(nullptr)
	{}

	opendnp3::MasterStackConfig config;
	std::string loggerId;
	opendnp3::ISOEHandler* SOEHandler;
	opendnp3::IMasterApplication* application;		
};

/**
* Callback interface invoked when a new connection is accepted
*/
class IListenCallback
{
public:

	virtual ~IListenCallback() {}

	/**
	* Ask user code if the following connection should be accepted
	*
	* @ipaddress The IP address of the connecting host. Can optionally be used for connection filtering.
	* @timeout The amount of time before a valid link frame is expected to be received. Can be overriden by the user callback.
	* 	
	* @return If true, the connection is accepted and a link frame parser is created to handle incoming frame data.
	*/ 
	virtual bool AcceptConnection(const std::string& ipaddress, openpal::TimeDuration& timeout) = 0;

	/**
	* Called the first time a frame is received from a newly accepted connection. This gives the 
	* application a chance to lookup information about the outstation based on the src/dest of the header. If a master
	* is created, the frame is passed up the new master's stack.
	* 
	* @param header the header from the first received link-layer frame
	* @param config Configuration information expected to be populated by the application code
	*/
	virtual bool CreateMasterSession(
		const opendnp3::LinkHeaderFields& header,
		MasterListenConfig& config
	) = 0;

	/**
	* Called when a new master is created after a successul call to CreateMasterSession()
	*/
	virtual void OnSessionCreated(IMaster& master) = 0;

	/**
	* Called when a new master is created after a successul call to OnCreateMaster()
	*/
	virtual bool OnSessionTerminated(IMaster& master) = 0; // TODO - provide statistics

};

}

#endif
