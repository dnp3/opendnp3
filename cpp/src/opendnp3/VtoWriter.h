
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
#ifndef __VTO_WRITER_H_
#define __VTO_WRITER_H_

#include <deque>
#include <set>
#include <mutex>

#include <opendnp3/IDataObserver.h>
#include <opendnp3/SubjectBase.h>
#include <openpal/Visibility.h>

#include "Loggable.h"
#include "EventTypes.h"
#include "VtoDataInterface.h"
#include "VtoData.h"
#include "IVtoEventAcceptor.h"

namespace opendnp3
{

/**
 * Implements the IVTOWriter interface that is handed out by the
 * stack.  Responsible for UserCode -> Stack thread marshalling and
 * stream decomposition.
 */
class DLL_LOCAL VtoWriter : public IVtoWriter, public SubjectBase, private Loggable
{
public:

	/**
	 * Creates a new VtoQueue instance.  The aMaxVtoChunks
	 * parameter defines the maximum number of 255-byte blocks
	 * that can be buffered at a time.
	 *
	 * @param aMaxVtoChunks	Maximum number of 255-byte blocks that
	 *                      can be stored at a time
	 *
	 * @return				the new VtoQueue instance
	 */
	VtoWriter(Logger* apLogger, size_t aMaxVtoChunks);

	~VtoWriter();

	/**
	* Registers an IVtoCallbacks to receive OnBufferAvailable() notifications
	* @param apHandler The interface to invoke when space is made available
	*/
	void AddVtoCallback(IVtoBufferHandler* apHandler);

	/**
	* Stops an IVtoCallbacks from receiving OnBufferAvailable() notifications
	* @param apHandler The interface to stop calling when space is available
	*/
	void RemoveVtoCallback(IVtoBufferHandler* apHandler);

	/**
	 * Implements IVtoWriter::Write().
	 */
	size_t Write(const uint8_t* apData,
	             size_t aLength,
	             uint8_t aChannelId);

	/**
	 * Implements IVtoWriter::SetLocalVtoState by shunting the state information to
	 * the magic vto channel 255
	 */
	virtual void SetLocalVtoState(bool aLocalVtoConnectionOpened,
	                              uint8_t aChannelId);
	/**
	 * Pulls items from the queue and pushes them to an IVtoEventAcceptor*
	 *
	 * @param apAcceptor	Interface that accepts the events
	 * @param aMaxEvents	The maximum number of events that will be written to apAcceptor
	 *
	 * @return				The number of events that were written
	 */
	size_t Flush(IVtoEventAcceptor* apAcceptor, size_t aMaxEvents);

	/**
	 * Implements IVtoWriter::Size().
	 */
	size_t Size();

	/**
	 * Returns the number of bytes available to the user
	 * application.  This is a sliding window, so the user
	 * application must manage the data length when calling
	 * VtoWriter::Write().
	 *
	 * @returns				the number of bytes free in the
	 *                      transmission queue
	 */
	size_t NumBytesAvailable();

protected:

	std::deque<VtoEvent> mQueue;

private:

	/**
	 * Lock used for thread safety
	 */
	std::mutex mMutex;

	/* implement ITransactable NVII functions */
	void _Start();
	void _End();

	void NotifyAllCallbacks();

	/**
	 * Returns the number of object chunks available in the
	 * transmission queue.
	 *
	 * @return			the free space available in the queue
	 */
	size_t NumChunksAvailable();

	void Commit(const uint8_t* apData,
	            size_t aLength,
	            uint8_t aChannelId);

	void QueueVtoObject(const uint8_t* apData,
	                    size_t aLength,
	                    uint8_t aChannelId);

	const size_t mMaxVtoChunks;

	typedef std::set<IVtoBufferHandler*> CallbackSet;
	CallbackSet mCallbacks;
};

}

/* vim: set ts=4 sw=4: */

#endif

