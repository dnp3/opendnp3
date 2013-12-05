#ifndef __LINK_FRAME_H_
#define __LINK_FRAME_H_

#include <openpal/Visibility.h>

#include "gen/FunctionCode.h"
#include "LinkHeader.h"

#include <string>

namespace opendnp3
{

class DLL_LOCAL LinkFrame
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
	inline LinkFunction	GetFunc() const {
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

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	/** @return String representation of the frame */
	std::string ToString() const;
#endif

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
	void FormatHeader(size_t aDataLength, bool aIsMaster, bool aFcb, bool aFcvDfc, LinkFunction aCode, uint16_t aDest, uint16_t aSrc);

	bool mIsComplete;
	size_t mSize;
	LinkHeader mHeader;
	uint8_t mpBuffer[LS_MAX_FRAME_SIZE];

};

}

#endif

