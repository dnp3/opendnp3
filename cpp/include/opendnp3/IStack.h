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
#ifndef __I_STACK_H_
#define __I_STACK_H_

#include <string>
#include <set>

#include "LogTypes.h"
#include "VtoRouterSettings.h"
#include "DestructorHook.h"
#include "StackState.h"

namespace boost
{
namespace asio
{
	class io_service;
}
}

namespace opendnp3
{

class Logger;
class IPhysicalLayerAsync;
class IVtoEndpoint;
class IVtoWriter;
class IVtoReader;
class VtoEndpointImpl;

/**
* Base class for masters or outstations. Can be used to bind a vto endpoint or shutdown.
*/
class IStack : public DestructorHook
{
	public:
		IStack(Logger* apLogger, boost::asio::io_service* apService);
		virtual ~IStack();

		/** 
		* Bind a vto endpoint as a tcp client
		*
		* @param arLoggerId Name used in all log messages
		* @param aLevel Lowest log level recorded
		* @param arHost IP address for connection
		* @param aPort port for connection
		* @param arSettings settings for the vto connection
		* @return interface representing the running endpoint
		*/
		IVtoEndpoint* StartVtoRouterTCPClient(const std::string& arLoggerId, FilterLevel aLevel, const std::string& arHost, uint16_t aPort, const VtoRouterSettings& arSettings);
		
		/** 
		* Bind a vto endpoint as a tcp server
		*
		* @param arLoggerId Name used in all log messages
		* @param aLevel Lowest log level recorded
		* @param arEndpoint Network adapter to bind to (i.e. 127.0.0.1 or 0.0.0.0)
		* @param aPort port to listen on
		* @param arSettings settings for the vto connection
		* @return interface representing the running endpoint
		*/
		IVtoEndpoint* StartVtoRouterTCPServer(const std::string& arLoggerId, FilterLevel aLevel, const std::string& arEndpoint, uint16_t aPort, const VtoRouterSettings& arSettings);

		/**
		* Add a listener for changes to the stack state. All callbacks come from the thread pool.
		* An immediate callback will be made with the current state.
		*
		* @param aListener Functor to call back with the state enumeration 
		*/
		virtual void AddStateListener(std::function<void (StackState)> aListener) = 0;

		/// Synchronously shutdown the endpoint
		virtual void Shutdown() = 0;	

	protected:	

		void CleanupVto();

		/**
		 * Returns a pointer to the IVtoWriter instance for the layer.
		 *
		 * @return		a pointer to the IVtoWriter for the layer
		 */
		virtual IVtoWriter* GetVtoWriter() = 0;

		/**
		 * Returns a pointer to the IVtoReader instance for the layer.
		 *
		 * @return		a pointer to the IVtoReader for the layer
		 */
		virtual IVtoReader* GetVtoReader() = 0;

	private:

		void OnVtoEndpointShutdown(VtoEndpointImpl* apEndpoint);

		IVtoEndpoint* CreateVtoEndpoint(IPhysicalLayerAsync* apPhys, const VtoRouterSettings& arSettings);

		Logger* mpLogger;
		boost::asio::io_service* mpService;
		std::set<IVtoEndpoint*> mVtoEndpoints;
};

}

#endif
