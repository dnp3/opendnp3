/**
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
#ifndef __LINK_LAYER_RECEIVER_H_
#define __LINK_LAYER_RECEIVER_H_


#include <openpal/BufferWrapper.h>
#include <openpal/StaticBuffer.h>
#include <openpal/Logger.h>

#include "opendnp3/DNPErrorCodes.h"

#include "opendnp3/link/ShiftableBuffer.h"
#include "opendnp3/link/LinkFrame.h"
#include "opendnp3/link/LinkHeader.h"
#include "opendnp3/StaticSizeConfiguration.h"


namespace opendnp3
{

class IFrameSink;
class LRS_Base;

/** Parses incoming ft3 frames for the link layer router.
*/
class LinkLayerReceiver
{

public:
	/**
		@param arLogger Logger that the receiver is to use.
		@param apSink Complete frames are sent to this interface.
	*/
	LinkLayerReceiver(const openpal::Logger& logger, IFrameSink* apSink);

	/**
		Called when valid data has been written to the current buffer write position
		@param aNumBytes Number of bytes written
	*/
	void OnRead(uint32_t aNumBytes);

	/**
	* Buffer that can currently be used for writing
	*/
	openpal::WriteBuffer WriteBuff() const;

	/**
		Resets the state of parser
	*/
	void Reset();

private:

	friend class LRS_Sync;
	friend class LRS_Header;
	friend class LRS_Body;

	//actions/helpers used by the states
	void ChangeState(LRS_Base* apState)
	{
		mpState = apState;
	}

	bool SyncStartOctets()
	{
		return mBuffer.Sync();
	}

	bool ReadHeader();
	bool ValidateBody();
	bool ValidateHeader();
	bool ValidateFunctionCode();
	void FailFrame();
	void PushFrame();
	openpal::ReadOnlyBuffer TransferUserData();

	uint32_t NumReadBytes() const
	{
		return mBuffer.NumReadBytes();
	}

	openpal::Logger logger;
	LinkHeader mHeader;
	uint32_t mFrameSize;
	static const uint8_t M_SYNC_PATTERN[2];

	IFrameSink* mpSink;  // pointer to interface to push complete frames
	LRS_Base* mpState;

	// Buffer to which user data is extracted, this is necessary since CRC checks are interlaced
	uint8_t mpUserData[LS_MAX_USER_DATA_SIZE];
	openpal::StaticBuffer<sizes::LINK_RECEIVER_BUFFER_SIZE> receiverBuffer;
	ShiftableBuffer mBuffer; //Buffer used to cache frames data as it arrives
};

}

#endif

