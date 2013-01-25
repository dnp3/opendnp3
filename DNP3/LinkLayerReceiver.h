
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
#ifndef __LINK_LAYER_RECEIVER_H_
#define __LINK_LAYER_RECEIVER_H_

#include <APL/Loggable.h>
#include <APL/Logger.h>
#include <APL/ShiftableBuffer.h>

#include "LinkFrame.h"
#include "DNPConstants.h"
#include "LinkHeader.h"

namespace apl
{
namespace dnp
{

class IFrameSink;
class LRS_Base;

/** Parses incoming ft3 frames for the link layer router.
*/
class LinkLayerReceiver : public Loggable
{
	static const size_t BUFFER_SIZE = (4096 / 249 + 1) * 292;

public:
	/**
		@param apLogger Logger that the receiver is to use.
		@param apSink Complete frames are sent to this interface.
	*/
	LinkLayerReceiver(Logger* apLogger, IFrameSink* apSink);

	/**
		Called when valid data has been written to the current buffer write position
		@param aNumBytes Number of bytes written
	*/
	void OnRead(size_t aNumBytes);

	/*
	// @return Returns true if a call to PushFrame() will succeed
	bool HasFrame();

	// Calls the router with the next frames header and/or payload
	void PushFrame();
	*/

	size_t NumWriteBytes() const {
		return mBuffer.NumWriteBytes();
	}
	uint8_t* WriteBuff() const {
		return mBuffer.WriteBuff();
	}

	//size_t NumReadBytes() const { return mBuffer.NumReadBytes(); }


private:

	friend class LRS_Sync;
	friend class LRS_Header;
	friend class LRS_Body;

	//actions/helpers used by the states
	void ChangeState(LRS_Base* apState) {
		mpState = apState;
	}
	bool Sync0564() {
		return mBuffer.Sync(M_SYNC_PATTERN, 2);
	}
	bool ReadHeader();
	bool ValidateBody();
	bool ValidateHeader();
	bool ValidateFunctionCode();
	void FailFrame();
	void PushFrame();
	size_t TransferUserData();
	size_t NumReadBytes() {
		return mBuffer.NumReadBytes();
	}

	LinkHeader mHeader;
	size_t mFrameSize;
	static const uint8_t M_SYNC_PATTERN[2];

	IFrameSink* mpSink;  // pointer to interface to push complete frames
	LRS_Base* mpState;

	// Buffer to which user data is extracted, this is necessary since CRC checks are interlaced
	uint8_t mpUserData[LS_MAX_USER_DATA_SIZE];
	ShiftableBuffer mBuffer; //Buffer used to cache frames data as it arrives	
};

}
}

#endif
