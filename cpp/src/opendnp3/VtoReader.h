
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
#ifndef __VTO_READER_H_
#define __VTO_READER_H_

#include <map>
#include <mutex>

#include <opendnp3/IDataObserver.h>
#include <openpal/Visibility.h>
#include <openpal/Loggable.h>

#include "VtoDataInterface.h"
#include "VtoData.h"

namespace opendnp3
{

/**
 * Class used to track registered VTO Channels, assemble VtoData
 * objects back into contigous streams, and deliver to the correct
 * channel
 */
class DLL_LOCAL VtoReader : private openpal::Loggable, public ITransactable, public IVtoReader
{
public:

	/**
	 * Create a new VtoReader instance.
	 *
	 * @param arLogger			the Logger that the instance
	 * 							should use for log messages
	 *
	 * @return					a new VtoReader instance
	 */
	VtoReader(openpal::Logger aLogger) : Loggable(aLogger) {}

	/**
	 * Register an IVtoCallbacks instance with the VtoReader
	 * instance.  The IVtoCallbacks instance is self-aware of its
	 * channel id.
	 *
	 * @param apCallbacks		The callback handler for the channel
	 *
	 * @throw ArgumentException	if the channel id is already
	 *registered
	 *                          with this reader
	 */
	void AddVtoChannel(IVtoDataHandler* apCallbacks);

	/**
	 * Unregister an IVtoCallbacks instance with the VtoReader
	 * instance.
	 *
	 * @param apCallbacks		The callback handler to unregister
	 *
	 * @throw ArgumentException	if the channel id is not registered
	 *                          with this reader
	 */
	void RemoveVtoChannel(IVtoDataHandler* apCallbacks);

	/**
	 * Adds a VtoEvent object to be delivered back to user code.
	 * Must be called from within a transaction block.  If a
	 * callback handler is not registered for the channel id,
	 * a log message will be recorded.
	 *
	 * If the aChannelId is the magic 255 we will parse the data
	 * as a message from a VtoWriter containing the remote vto
	 * connection state.
	 *
	 * @param arData			the data waiting to be delivered
	 * @param aChannelId		the channel id on which the data was
	 *                          received
	 */
	void Update(const VtoData& arData, uint8_t aChannelId);

protected:

	/**
	 * The ITransactable transaction lock.
	 */
	std::mutex mMutex;

private:

	void UpdateEnhancedVto(const VtoData& arData, uint8_t aChannelId);

	void UpdateNormalVto(const VtoData& arData, uint8_t aChannelId);

	/**
	 * Starts the ITransactable transaction lock.
	 */
	void _Start();

	/**
	 * Ends the ITransactable transaction lock.
	 */
	void _End();

	typedef std::map<uint8_t, IVtoDataHandler*> ChannelMap;

	ChannelMap mChannelMap;
};

}

/* vim: set ts=4 sw=4: */

#endif

