
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __ENHANCED_VTO_ROUTER_H_
#define __ENHANCED_VTO_ROUTER_H_

#include "VtoRouter.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

/**
 * Base class for the VtoRouters that take advantage of the extended
 * callbacks to manage the local side of a tunneled connection. This
 * base class provides the common machinery for generating and recieving
 * notiifications that the local/remote connection has been created or
 * terminated. The concrete base classes provide policy for when to start
 * or stop the local side of the connection.
 */
class DLL_LOCAL EnhancedVtoRouter : public VtoRouter
{
public:

	EnhancedVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer);

	// these base functions handle updating the state variables
	// and inform the remote side of local connection changes
	void DoVtoRemoteConnectedChanged(bool aOpened);
	void SetLocalConnected(bool aConnected);



protected:

	void CloseAndFlushBuffers();

	// concrete classes use these callbacks and the state
	// variables to decide when to Start or Stop attempting
	// the local connections
	virtual void HandleVtoRemoteConnectedChanged() = 0;
	virtual void HandleSetLocalConnected() = 0;
	virtual void HandleReceivingDataWhenRemoteClosed() = 0;
	virtual void HandleDuplicateOpen() = 0;
	virtual void HandleDuplicateClose() = 0;

	bool CheckIncomingVtoData(const VtoData& arData);

	bool mRemoteConnected;
	bool mLocalConnected;

private:



	static std::string GetConnectionString(bool aOpen);
	bool mInstRemoteConnected;

};

/**
 * The server socket vto implementation will always keep the local side of the
 * connection running. If the remote connection drops we will drop the local
 * connection if it exists to indicate to our client that the remote end
 * terminated the connection. An example where this is useful is when SSH is being
 * tunneled and the user uses the "logout" command, the ssh server closes
 * the socket to inform the client that the session is over. It can be paired
 * with a remote ClientSocket or AlwaysOnline vto router, but not another
 * ServerSocket router.
 */
class DLL_LOCAL ServerSocketVtoRouter : public EnhancedVtoRouter
{
public:

	ServerSocketVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer);

	void HandleVtoRemoteConnectedChanged();
	void HandleSetLocalConnected();
	void HandleReceivingDataWhenRemoteClosed();
	void HandleDuplicateOpen();
	void HandleDuplicateClose();

};

/**
 * The client socket vto implementation will only attempt to open a client
 * connection when the remote side indicates that a connection has been made
 * on that side. When a "remote connected"  message is received it will respond
 * with a "local connected" message and only if it can't connect locally will it
 * send a "local disconnected" message.
 */
class DLL_LOCAL ClientSocketVtoRouter : public EnhancedVtoRouter
{
public:

	ClientSocketVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer);

	void HandleVtoRemoteConnectedChanged();
	void HandleSetLocalConnected();
	void HandleReceivingDataWhenRemoteClosed();
	void HandleDuplicateOpen();
	void HandleDuplicateClose();
};

}

/* vim: set ts=4 sw=4: */

#endif

