
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
#ifndef __LINK_FRAME_H_
#define __LINK_FRAME_H_

#include "LinkLayerConstants.h"
#include "PackingUnpacking.h"
#include "LinkHeader.h"

#include <string>

namespace opendnp3
{

class LinkFrame
{
	friend std::ostream& operator<<(std::ostream&, const LinkFrame&);
	friend class FrameReaderDNP;

public:

	bool operator==(const LinkFrame& arRHS) const;

	LinkFrame();
	~LinkFrame();

	/* implement pure virtual functions from base class */
	uint8_t* GetBuffer() {
		return mpBuffer;
	}
	const uint8_t* GetBuffer() const {
		return mpBuffer;
	}
	size_t  GetSize() const {
		return mSize;
	}

	bool IsComplete() const {
		return mIsComplete;
	}

	bool ValidateHeaderCRC() const;
	bool ValidateBodyCRC() const;

	// Helpers for extracting data
	inline bool Valid_0564() const {
		return mpBuffer[LI_START_05] == 0x05 && mpBuffer[LI_START_64] == 0x64;
	}

	inline uint8_t	GetLength() const {
		return mHeader.GetLength();
	}
	inline FuncCodes	GetFunc() const {
		return mHeader.GetFuncEnum();
	}

	inline uint16_t  GetDest() const {
		return mHeader.GetDest();
	}
	inline uint16_t GetSrc() const {
		return mHeader.GetSrc();
	}

	inline bool IsFromMaster() const {
		return mHeader.IsFromMaster();
	}
	inline bool IsPriToSec() const {
		return mHeader.IsPriToSec();
	}
	inline bool IsFcbSet() const {
		return mHeader.IsFcbSet();
	}
	inline bool IsFcvDfcSet() const {
		return mHeader.IsFcvDfcSet();
	}

	////////////////////////////////////////////////
	//	Functions for formatting outgoing Sec to Pri frames
	////////////////////////////////////////////////

	void FormatAck(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void FormatNack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void FormatLinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void FormatNotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);

	////////////////////////////////////////////////
	//	Functions for formatting outgoing Pri to Sec frames
	////////////////////////////////////////////////

	void FormatTestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	void FormatResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void FormatRequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void FormatConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength);
	void FormatUnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength);

	void ChangeFCB(bool aFCB);

	////////////////////////////////////////////////
	//	Reusable static formatting functions to any buffer
	////////////////////////////////////////////////

	// @return Total frame size based on user data length
	static size_t CalcFrameSize(size_t aDataLength);

	/** @return String representation of the frame */
	std::string ToString() const;

	/** Validates FT3 user data integriry
		@param apBody Beginning of the FT3 user data
		@param aLength Number of user bytes to verify, not user + crc.
		@return True if the body CRC is correct */
	static bool ValidateBodyCRC(const uint8_t* apBody, size_t aLength);

	/** Reads data from src to dest removing 2 byte CRC checks every 16 data bytes
		@param apSrc Source buffer with crc checks. Must begin at data, not header
		@param apDest Destination buffer to which the data is extracted
		@param aLength Length of user data to read to the dest buffer. The source buffer must be larger b/c of crc bytes.
	*/
	static void ReadUserData(const uint8_t* apSrc, uint8_t* apDest, size_t aLength);

private:

	/** Writes data from src to dest interlacing 2 byte CRC checks every 16 data bytes
		@param apSrc Source buffer full of user data
		@param apDest Destination buffer where the data + CRC is written
		@param aLength Number of user data bytes
	*/
	static void WriteUserData(const uint8_t* apSrc, uint8_t* apDest, size_t aLength);

	/** Write 10 header bytes to to buffer including 0x0564, all fields, and CRC */
	void FormatHeader(size_t aDataLength, bool aIsMaster, bool aFcb, bool aFcvDfc, FuncCodes aCode, uint16_t aDest, uint16_t aSrc);

	bool mIsComplete;
	size_t mSize;
	LinkHeader mHeader;
	uint8_t mpBuffer[LS_MAX_FRAME_SIZE];

};

}

#endif

