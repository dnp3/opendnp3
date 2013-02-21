
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
#include "VtoWriter.h"


#include <opendnp3/Logger.h>
#include <opendnp3/Util.h>

#include "EnhancedVto.h"
#include "LoggableMacros.h"

namespace opendnp3
{

VtoWriter::VtoWriter(Logger* apLogger, size_t aMaxVtoChunks) :
	Loggable(apLogger),
	mMaxVtoChunks(aMaxVtoChunks)
{}

VtoWriter::~VtoWriter()
{
	if(mQueue.size() > 0) {
		LOG_BLOCK(LEV_WARNING, "On destruction, writer had " << mQueue.size() << " chunks that went unread");
	}
}

size_t VtoWriter::Write(const uint8_t* apData,
                        size_t aLength,
                        uint8_t aChannelId)
{

	size_t num = 0;
	{
		std::unique_lock<std::mutex> lock(mMutex);

		/*
		 * Only write the maximum amount available or requested.  If the
		 * requested data size is larger than the available buffer space,
		 * only send what will fit.
		 */
		num = Min<size_t>(this->NumBytesAvailable(), aLength);

		/*
		 * Chop up the data into Max(255) segments and add it to the queue.
		 */
		this->Commit(apData, num, aChannelId);
	}

	/* Tell any listeners that the queue has new data to be read. */
	if (num > 0) this->NotifyObservers();

	/* Return the number of bytes from apData that were queued. */
	return num;
}

void VtoWriter::SetLocalVtoState(bool aLocalVtoConnectionOpened, uint8_t aChannelId)
{

	VtoData vto = EnhancedVto::CreateVtoData(aLocalVtoConnectionOpened, aChannelId);
	VtoEvent evt(vto, PC_CLASS_1, 255);

	/* Thread safe for rest of function */
	std::unique_lock<std::mutex> lock(mMutex);
	this->mQueue.push_back(evt);
	this->NotifyObservers();
}

void VtoWriter::Commit(const uint8_t* apData,
                       size_t aLength,
                       uint8_t aChannelId)
{
	/*
	 * The data is segmented into N number of chunks, each of MAX_SIZE
	 * bytes, and 1 chunk containing the remainder of the data that is
	 * less than MAX_SIZE bytes.  We pre-calculate the chunk size to
	 * avoid the constant comparison overhead in the loop itself.
	 */
	size_t complete = aLength / VtoData::MAX_SIZE;
	size_t partial = aLength % VtoData::MAX_SIZE;

	/* First, write the full-sized blocks */
	for (size_t i = 0; i < complete; ++i) {
		this->QueueVtoObject(apData, VtoData::MAX_SIZE, aChannelId);
		apData += VtoData::MAX_SIZE;
	}

	/* Next, write the remaining data at the end of the stream */
	if (partial > 0)
		this->QueueVtoObject(apData, partial, aChannelId);
}

void VtoWriter::QueueVtoObject(const uint8_t* apData,
                               size_t aLength,
                               uint8_t aChannelId)
{
	/*
	 * Create a new VtoData instance, set the event data associated
	 * with it, and then push the object onto the transmission queue.
	 */
	VtoData vto(apData, aLength);

	VtoEvent evt(vto, PC_CLASS_1, aChannelId);
	this->mQueue.push_back(evt);
}

size_t VtoWriter::Flush(IVtoEventAcceptor* apAcceptor, size_t aMaxEvents)
{
	size_t numUpdates = 0;

	{
		std::unique_lock<std::mutex> lock(mMutex);
		while(numUpdates < aMaxEvents && mQueue.size() > 0) {
			VtoEvent& evt = mQueue.front();
			apAcceptor->Update(evt.mValue, evt.mClass, evt.mIndex);
			mQueue.pop_front();
			++numUpdates;
		}
	}


	if(numUpdates > 0) this->NotifyAllCallbacks();

	return numUpdates;
}

void VtoWriter::AddVtoCallback(IVtoBufferHandler* apHandler)
{
	assert(apHandler != NULL);
	std::unique_lock<std::mutex> lock(mMutex);
	this->mCallbacks.insert(apHandler);
}

void VtoWriter::RemoveVtoCallback(IVtoBufferHandler* apHandler)
{
	assert(apHandler != NULL);
	std::unique_lock<std::mutex> lock(mMutex);
	this->mCallbacks.erase(apHandler);
}

void VtoWriter::NotifyAllCallbacks()
{
	CallbackSet set; //create a copy of the set outside the critical section
	{
		std::unique_lock<std::mutex> lock(mMutex);
		set = mCallbacks;
	}

for(auto pCallback: set) pCallback->OnBufferAvailable();
}

size_t VtoWriter::Size()
{
	/*
	 * The whole function is thread-safe, from start to finish.
	 */
	std::unique_lock<std::mutex> lock(mMutex);

	return mQueue.size();
}

size_t VtoWriter::NumChunksAvailable()
{
	return mMaxVtoChunks - mQueue.size();
}

size_t VtoWriter::NumBytesAvailable()
{
	return this->NumChunksAvailable() * VtoData::MAX_SIZE;
}

}

/* vim: set ts=4 sw=4: */

