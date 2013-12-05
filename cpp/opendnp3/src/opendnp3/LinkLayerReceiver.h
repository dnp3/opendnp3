#ifndef __LINK_LAYER_RECEIVER_H_
#define __LINK_LAYER_RECEIVER_H_


#include <opendnp3/DNPConstants.h>
#include <openpal/Visibility.h>
#include <openpal/Loggable.h>
#include <openpal/BufferWrapper.h>

#include "ShiftableBuffer.h"
#include "LinkFrame.h"
#include "LinkHeader.h"


namespace opendnp3
{

class IFrameSink;
class LRS_Base;

/** Parses incoming ft3 frames for the link layer router.
*/
class DLL_LOCAL LinkLayerReceiver : public openpal::Loggable
{
	static const size_t BUFFER_SIZE = (4096 / 249 + 1) * 292;

public:
	/**
		@param arLogger Logger that the receiver is to use.
		@param apSink Complete frames are sent to this interface.
	*/
	LinkLayerReceiver(openpal::Logger aLogger, IFrameSink* apSink);

	/**
		Called when valid data has been written to the current buffer write position
		@param aNumBytes Number of bytes written
	*/
	void OnRead(size_t aNumBytes);
	
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
	openpal::ReadOnlyBuffer TransferUserData();
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

#endif

