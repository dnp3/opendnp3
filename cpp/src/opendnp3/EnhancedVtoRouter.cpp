
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "EnhancedVtoRouter.h"

#include "Exception.h"
#include "Logger.h"
#include "LoggableMacros.h"

namespace opendnp3
{

EnhancedVtoRouter::EnhancedVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer) :
	Loggable(apLogger),
	VtoRouter(arSettings, apLogger, apWriter, apPhysLayer),
	mRemoteConnected(false),
	mLocalConnected(false),
	mInstRemoteConnected(false)
{

}

bool EnhancedVtoRouter::CheckIncomingVtoData(const VtoData& arData)
{
	switch(arData.GetType()) {
	case(VTODT_DATA):
		if(mInstRemoteConnected) return true;
		else {
			LOG_BLOCK(LEV_WARNING, "Discarding received data, because remote side is offline");
			this->HandleReceivingDataWhenRemoteClosed();
			return false;
		}
	case(VTODT_REMOTE_OPENED):
		if(mInstRemoteConnected) {
			LOG_BLOCK(LEV_WARNING, "Remote side opened, but it was already open");
			this->HandleDuplicateOpen();
			return false;
		}
		else {
			mInstRemoteConnected = true;
			return true;
		}
	case(VTODT_REMOTE_CLOSED):
		if(mInstRemoteConnected) {
			mInstRemoteConnected = false;
			return true;
		}
		else {
			LOG_BLOCK(LEV_WARNING, "Remote side closed, but it was already closed");
			this->HandleDuplicateClose();
			return false;
		}
	default:
		throw ArgumentException(LOCATION, "Unknown VtoData type");
	}
}

std::string EnhancedVtoRouter::GetConnectionString(bool aOpen)
{
	return aOpen ? "OPEN" : "CLOSED";
}

void EnhancedVtoRouter::DoVtoRemoteConnectedChanged(bool aOpened)
{
	if(mRemoteConnected != aOpened) {
		LOG_BLOCK(LEV_INFO, "Remote connection: " << GetConnectionString(aOpened));
		mRemoteConnected = aOpened;
		this->HandleVtoRemoteConnectedChanged();
	}
}

void EnhancedVtoRouter::SetLocalConnected(bool aConnected)
{
	if(mLocalConnected != aConnected) {
		LOG_BLOCK(LEV_INFO, "Local connection: " << GetConnectionString(aConnected));
		mLocalConnected = aConnected;
		this->HandleSetLocalConnected();
		this->NotifyRemoteSideOfState(aConnected);
	}
}

void EnhancedVtoRouter::CloseAndFlushBuffers()
{
	FlushBuffers();
	this->Close();
}

/*****************************************
 * Server socket specific implementation
 *************************************/

ServerSocketVtoRouter::ServerSocketVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer) :
	Loggable(apLogger),
	EnhancedVtoRouter(arSettings, apLogger, apWriter, apPhysLayer)
{
	this->Start();
}

void ServerSocketVtoRouter::HandleVtoRemoteConnectedChanged()
{
	if(!mRemoteConnected) {
		// if the remote side has closed we should close our
		// local connection and then prepare for a new one
		this->FlushBuffers();
		this->Close();
	}

}

void ServerSocketVtoRouter::HandleSetLocalConnected()
{
	if(!mLocalConnected) this->FlushBuffers();
}

void ServerSocketVtoRouter::HandleReceivingDataWhenRemoteClosed()
{

	if(this->mLocalConnected) {
		this->FlushBuffers();
		this->Close();
	}
	else {
		this->NotifyRemoteSideOfState(false);
	}
}

void ServerSocketVtoRouter::HandleDuplicateOpen()
{
	if(this->mLocalConnected) {
		this->FlushBuffers();
		this->Close();
	}
}

void ServerSocketVtoRouter::HandleDuplicateClose()
{
	if(this->mLocalConnected) {
		this->FlushBuffers();
		this->Close();
	}
}

/*****************************************
 * Client socket specific implementation
 *************************************/
ClientSocketVtoRouter::ClientSocketVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer) :
	Loggable(apLogger),
	EnhancedVtoRouter(arSettings, apLogger, apWriter, apPhysLayer)
{

}

void ClientSocketVtoRouter::HandleVtoRemoteConnectedChanged()
{
	if(mRemoteConnected) {
		// pretend we are online, so the other side sees a "connected"
		// message. If we succeed in connecting the second SetLocalConnected(true)
		// call will do nothing, if we fail the SetLocalConnected(false) call
		// will cause a "disconnected" message to be sent so the server side
		// can close the connection. Effectivley we tunnel the "connection
		// refused" behavior as a "remote server terminated connection"
		this->SetLocalConnected(true);
		this->StartOne();
	}
	else {
		this->FlushBuffers();
		this->Suspend(); //stop local connection attempts when we lose the remote side
	}
}

void ClientSocketVtoRouter::HandleSetLocalConnected()
{
	// we shouldn't automatically reconnect when the connection drops
	if(!mLocalConnected) this->CloseAndFlushBuffers();
}

void ClientSocketVtoRouter::HandleReceivingDataWhenRemoteClosed()
{
	if(this->mLocalConnected) {
		this->CloseAndFlushBuffers();
	}
	else {
		this->NotifyRemoteSideOfState(false);
	}
}

void ClientSocketVtoRouter::HandleDuplicateOpen()
{
	if(this->mLocalConnected) {
		this->CloseAndFlushBuffers();
	}
}

void ClientSocketVtoRouter::HandleDuplicateClose()
{

}

}

/* vim: set ts=4 sw=4: */

