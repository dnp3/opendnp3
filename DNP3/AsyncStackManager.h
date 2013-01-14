
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
#ifndef __ASYNC_STACK_MANAGER_H_
#define __ASYNC_STACK_MANAGER_H_

#include <map>
#include <vector>
#include <mutex>

#include <APL/Loggable.h>
#include <APL/DataInterfaces.h>
#include <APL/IPhysicalLayerObserver.h>
#include <APL/PhysicalLayerManager.h>
#include <APL/AsyncTaskScheduler.h>
#include <APL/IOServiceThreadPool.h>

#include "VtoDataInterface.h"
#include "LinkRoute.h"
#include "VtoRouterManager.h"

namespace apl
{
class IPhysicalLayerAsync;
class Logger;
class ICommandAcceptor;
class IDataObserver;
}

namespace apl
{
namespace dnp
{

class LinkChannel;
class Stack;
struct VtoRouterSettings;

struct SlaveStackConfig;
struct MasterStackConfig;

/**
	The interface for C++ projects for dnp3. Provides an interface for
	starting/stopping master/slave protocol stacks. Any method may be
	called while the system is running.  Methods should only be called
	from a single thread at at a time.
*/
class AsyncStackManager : private Loggable
{
public:
	/**
		@param apLogger - Logger to use for all other loggers
	*/
	AsyncStackManager(Logger* apLogger, size_t aConcurreny);
	~AsyncStackManager();

	// All the io_service marshalling now occurs here. It's now safe to add/remove while the manager is running.

	// Adds a TCPClient port, excepts if the port already exists
	void AddTCPClient(const std::string& arName, PhysLayerSettings, const std::string& arAddr, uint16_t aPort);

	// Adds a TCPServer port, excepts if the port already exists
	void AddTCPServer(const std::string& arName, PhysLayerSettings, const std::string& arEndpoint, uint16_t aPort);

	// Adds a Serial port, excepts if the port already exists
	void AddSerial(const std::string& arName, PhysLayerSettings, SerialSettings);

	// Adds a custom physical layer to the stack
	void AddPhysicalLayer(const std::string&, PhysLayerSettings, IPhysicalLayerAsync* apPhys);

	/**
		Adds a master stack - Stack will automatically start running if
		Start() has been called or aAutoRun is true.

		@param arPortName			Unique name of the port with which to
									associate the stack.
		@param arStackName			Unique name of the stack.
		@param aLevel				Log filter level to use.
		@param apPublisher			Interface to callback with measurements.
									The callback comes from an unknown
									network thread and should not be
									blocked.
		@param arCfg				Configuration data for the master stack

		@return						Thread-safe interface to use for
									dispatching commands to the master.

		@throw ArgumentException	if arPortName doesn't exist or if
									arStackName already exists
	*/
	ICommandAcceptor* AddMaster(const std::string& arPortName,
	                            const std::string& arStackName,
	                            FilterLevel aLevel,
	                            IDataObserver* apPublisher,
	                            const MasterStackConfig& arCfg);

	/**
		Adds a slave stack - Stack will automatically start running if
		Start() has been called or aAutoRun is true.

		@param arPortName			Unique name of the port with which to
									associate the stack.
		@param arStackName			Unique name of the stack.
		@param aLevel				Log filter level to use.
		@param apCmdAcceptor		Interface to callback with measurements.
									The callback comes from an unknown
									network thread and should not be
									blocked.
		@param arCfg				Configuration data for the master stack

		@return						Thread-safe interface to use for
									writing new measurement values to the
									slave

		@throw ArgumentException	if arPortName doesn't exist or if
									arStackName already exists
	*/
	IDataObserver* AddSlave(const std::string& arPortName,
	                        const std::string& arStackName,
	                        FilterLevel aLevel,
	                        ICommandAcceptor* apCmdAcceptor,
	                        const SlaveStackConfig&);

	/**
		Adds a VTO channel to a prexisting stack (master or slave).
		This function should be used for advanced control of a VTO channel,
		where the implementer wants to control the end VTO stream as a byte
		array.  Otherwise, the implementer should look at
		AsyncStackManager::StartVtoRouter() as a simpler way of connecting a
		port (such as a local TCP service) to the VTO stream.

		@param arStackName			Unique name of the stack.
		@param apOnDataCallback		Interface to callback with received
									data.  The callback comes from an
									unknown network thread, and should not
									be blocked.

		@return						Interface to use for writing
									new VTO data from the master to the
									slave.

		@throw ArgumentException	if arPortName/arStackName doesn't exist
									or if the VTO channel ID is already
									bound for that stack
	 */
	void AddVtoChannel(const std::string& arStackName,
	                   IVtoCallbacks* apOnDataCallback);

	/**
		Removes an existing VTO channel that was created using
		AsyncStackManager::AddVtoChannel(), stopping callbacks.

		@param arStackName			Unique name of the stack.
		@param apOnDataCallback		Callback interface previously registered
									in AddVtoChannel()

		@throw ArgumentException if apOnDataCallback doesn't exist
	*/
	void RemoveVtoChannel(const std::string& arStackName, IVtoCallbacks* apOnDataCallback);

	/**
		Starts the VtoRouter for the specified port and stack.
		A VtoRouter acts as a conduit, where the VTO stream is funneled
		between the arStackName (which also defines a master/slave port) and
		the arPortName (such as a TCP/IP daemon).  If the implementer wants
		to terminate the VTO stream in the application itself, he/she should
		look at the AsyncStackManager::AddVtoChannel() routine.

		@param arPortName			Unique name of the port to which the
									router should associate.
		@param arStackName			Unique name of the stack.
		@param arSettings			Configuration class for the router.

		@return						Interface to use for writing
									new VTO data from the master to the
									slave.

		@throw ArgumentException	if arPortName/arStackName doesn't exist
									or if the VTO channel ID is already
									bound for that stack
	 */
	void StartVtoRouter(const std::string& arPortName,
	                    const std::string& arStackName,
	                    const VtoRouterSettings& arSettings);

	/**
		Shutdown a VtoRouter for the VTO channel on the specified stack.

		@param arStackName			Unique name of the stack.
		@param aVtoChannelId		Unique channel ID for the VTO circuit.

		@throw ArgumentException	if arStackName or the VTO channel ID
									does not exist
	*/
	void StopVtoRouter(const std::string& arStackName,
	                   uint8_t aVtoChannelId);

	/**
		Shutdown all VtoRouter instances on the specified stack.

		@param arStackName			Unique name of the stack.

		@throw ArgumentException	if arStackName doesn't exist
	*/
	void StopAllRoutersOnStack(const std::string& arStackName);

	/**
		Get the vto writer by stack name

		@param arStackName Unique name of the stack

		@throw ArgumentException if arStackName doesn't exist

		@return The IVtoWriter interface for the named stack
	*/
	IVtoWriter* GetVtoWriter(const std::string& arStackName);

	// Synchronously remove a port and all associated stacks
	void RemovePort(const std::string& arPortName);

	// Synchronously remove a single stack
	void RemoveStack(const std::string& arStackName);

	// @return a vector of all the stack names
	std::vector<std::string> GetStackNames();

	// @return a vector of all the port names
	std::vector<std::string> GetPortNames();

	/**
	* Synchronously stops all running stacks and ports. Permanently
	* stops the running background thread.
	*/
	void Shutdown();

	boost::asio::io_service* GetIOService()
	{
		return this->mPool.GetIOService();
	}

private:
	
	void OnPreStackDeletion(Stack* apStack);

	// Remove and close a stack, but delegate responsibility for deletion
	Stack* SeverStackFromChannel(const std::string& arStackName);

	// Add a stack from to a specified channel
	void AddStackToChannel(const std::string& arStackName, Stack* apStack, LinkChannel* apChannel, const LinkRoute& arRoute);

	IOServiceThreadPool mPool;
	
	PhysicalLayerManager mMgr;
	AsyncTaskScheduler mScheduler;
	VtoRouterManager mVtoManager;	
	bool mIsShutdown;

	void ThrowIfAlreadyShutdown();

	struct StackRecord {
		StackRecord() :
			stack(NULL), channel(NULL)
		{}

		StackRecord(Stack* apStack, LinkChannel* apChannel) :
			stack(apStack), channel(apChannel)
		{}

		Stack* stack;
		LinkChannel* channel;
	};

	typedef std::map<std::string, StackRecord> StackMap; // maps a stack name the stack and it's channel
	StackMap mStackMap;

	typedef std::map<std::string, LinkChannel*> ChannelToChannelMap;
	ChannelToChannelMap mChannelNameToChannel;	// maps a channel name to a channel instance

	LinkChannel* GetOrCreateChannel(const std::string& arName);
	LinkChannel* GetChannelOrExcept(const std::string& arName);
	LinkChannel* GetChannelMaybeNull(const std::string& arName);
	LinkChannel* CreateChannel(const std::string& arName);

	StackRecord GetStackRecordByName(const std::string& arName);




};

}
}

/* vim: set ts=4 sw=4: */

#endif
