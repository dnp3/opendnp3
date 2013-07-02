
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
// you under the terms of the License.
//

#include <assert.h>

#include <opendnp3/Logger.h>

#include "PackingUnpacking.h"
#include "LoggableMacros.h"
#include "DNPCrc.h"
#include "LinkReceiverStates.h"
#include "IFrameSink.h"

namespace opendnp3
{

const uint8_t LinkLayerReceiver::M_SYNC_PATTERN[2] = {0x05, 0x64};

LinkLayerReceiver::LinkLayerReceiver(Logger* apLogger, IFrameSink* apSink) :
	Loggable(apLogger),
	mFrameSize(0),
	mpSink(apSink),
	mpState(LRS_Sync::Inst()),
	mBuffer(BUFFER_SIZE)
{

}

void LinkLayerReceiver::OnRead(size_t aNumBytes)
{
	// This is a serious condition if it occurs
	// It indicates a possible buffer over run
	assert(aNumBytes <= mBuffer.NumWriteBytes());
	mBuffer.AdvanceWrite(aNumBytes);

	// this might push frame data to the sink and will free
	// space in the buffer
	while(mpState->Parse(this));

	//anytime we have a partially incomplete frame, shift the buffer
	mBuffer.Shift();
}

void LinkLayerReceiver::PushFrame()
{
	switch(mHeader.GetFuncEnum()) {
	case(FC_PRI_RESET_LINK_STATES):
		mpSink->ResetLinkStates(mHeader.IsFromMaster(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(FC_PRI_TEST_LINK_STATES):
		mpSink->TestLinkStatus(mHeader.IsFromMaster(), mHeader.IsFcbSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(FC_PRI_CONFIRMED_USER_DATA):
		mpSink->ConfirmedUserData(mHeader.IsFromMaster(), mHeader.IsFcbSet(), mHeader.GetDest(), mHeader.GetSrc(),
		                          mpUserData, TransferUserData());
		break;
	case(FC_PRI_UNCONFIRMED_USER_DATA):
		mpSink->UnconfirmedUserData(mHeader.IsFromMaster(), mHeader.GetDest(), mHeader.GetSrc(),
		                            mpUserData, TransferUserData());
		break;
	case(FC_PRI_REQUEST_LINK_STATUS):
		mpSink->RequestLinkStatus(mHeader.IsFromMaster(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(FC_SEC_ACK):
		mpSink->Ack(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(FC_SEC_NACK):
		mpSink->Nack(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(FC_SEC_LINK_STATUS):
		mpSink->LinkStatus(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(FC_SEC_NOT_SUPPORTED):
		mpSink->NotSupported(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	default:
		break;
	}

	mBuffer.AdvanceRead(mFrameSize);
}

size_t LinkLayerReceiver::TransferUserData()
{
	size_t len = mHeader.GetLength() - LS_MIN_LENGTH;
	LinkFrame::ReadUserData(mBuffer.ReadBuff() + LS_HEADER_SIZE, mpUserData, len);
	return len;
}

bool LinkLayerReceiver::ReadHeader()
{
	mHeader.Read(mBuffer.ReadBuff());
	return this->ValidateHeader();
}

bool LinkLayerReceiver::ValidateBody()
{
	size_t len = mHeader.GetLength() - LS_MIN_LENGTH;
	if(LinkFrame::ValidateBodyCRC(mBuffer.ReadBuff() + LS_HEADER_SIZE, len)) return true;
	else {
		ERROR_BLOCK(LEV_ERROR, "CRC failure in body", DLERR_CRC);
		return false;
	}
}

bool LinkLayerReceiver::ValidateHeader()
{
	//first thing to do is check the CRC
	if(!DNPCrc::IsCorrectCRC(mBuffer.ReadBuff(), LI_CRC)) {
		ERROR_BLOCK(LEV_ERROR, "CRC failure in header", DLERR_CRC);
		return false;
	}

	if(!mHeader.ValidLength()) {
		ERROR_BLOCK(LEV_ERROR, "LENGTH out of range [5,255]: " << static_cast<int>(mHeader.GetLength()), DLERR_INVALID_LENGTH);
		return false;
	}

	LOG_BLOCK(LEV_INTERPRET, "<~ " << mHeader.ToString());

	// some combinations of these header parameters are invalid
	// check for them here

	//Now make sure that the function code is known and that the FCV is appropriate
	if(!this->ValidateFunctionCode()) return false;

	uint8_t user_data_length = mHeader.GetLength() - LS_MIN_LENGTH;
	mFrameSize = LinkFrame::CalcFrameSize(user_data_length);
	FuncCodes func = mHeader.GetFuncEnum();

	// make sure that the presence/absence of user data
	// matches the function code
	if(func == FC_PRI_CONFIRMED_USER_DATA || func == FC_PRI_UNCONFIRMED_USER_DATA) {
		if(user_data_length > 0) {
			//mFrameSize = LinkFrame::CalcFrameSize(user_data_length);
		}
		else {
			ERROR_BLOCK(LEV_ERROR, "User data packet received with zero payload. FUNCTION: " << func, DLERR_NO_DATA);
			return false;
		}
	}
	else {
		if(user_data_length > 0) {
			ERROR_BLOCK(LEV_ERROR, "Unexpected LENGTH in frame: " << static_cast<int>(user_data_length) << " with FUNCTION: " << func, DLERR_UNEXPECTED_DATA);
			return false;
		}

	}

	if(user_data_length > 0) {
		if(func == FC_PRI_CONFIRMED_USER_DATA || func == FC_PRI_UNCONFIRMED_USER_DATA) {

		}
		else {
			ERROR_BLOCK(LEV_ERROR, "Unexpected LENGTH in frame: " << static_cast<int>(user_data_length) << " with FUNCTION: " << func, DLERR_UNEXPECTED_DATA);
			return false;
		}
	}
	else {
		if(func == FC_PRI_CONFIRMED_USER_DATA || func == FC_PRI_UNCONFIRMED_USER_DATA) {
			ERROR_BLOCK(LEV_ERROR, "User data packet received with zero payload. FUNCTION: " << func, DLERR_NO_DATA);
			return false;
		}
	}

	return true;
}

void LinkLayerReceiver::FailFrame()
{
	// All you have to do is advance the reader by one, when the resync happens the data will disappear
	mBuffer.AdvanceRead(1);
}

bool LinkLayerReceiver::ValidateFunctionCode()
{
	//Now make sure that the function code is known and that the FCV is appropriate
	if(mHeader.IsPriToSec()) {
		bool fcv_set = false;

		switch(mHeader.GetFuncEnum()) {
		case(FC_PRI_CONFIRMED_USER_DATA):
		case(FC_PRI_TEST_LINK_STATES):
			fcv_set = true;
			break;
		case(FC_PRI_REQUEST_LINK_STATUS):
		case(FC_PRI_RESET_LINK_STATES):
		case(FC_PRI_UNCONFIRMED_USER_DATA):
			break;
		default: {
				ERROR_BLOCK(LEV_WARNING, "Unknown PriToSec FUNCTION: " << mHeader.GetFuncEnum(), DLERR_UNKNOWN_FUNC);
				return false;
			}
		}

		//now check the fcv
		if(fcv_set != mHeader.IsFcvDfcSet()) {
			ERROR_BLOCK(LEV_WARNING, "Bad FCV for FUNCTION: " << mHeader.GetFuncEnum(), DLERR_UNEXPECTED_FCV);
			return false;
		}

		//if fcv isn't expected to be set, fcb can be either 1 or 0, doesn't matter

	}
	else { // SecToPri - just validate the function codes and that FCB is 0
		switch(mHeader.GetFuncEnum()) {
		case(FC_SEC_ACK):
		case(FC_SEC_NACK):
		case(FC_SEC_LINK_STATUS):
		case(FC_SEC_NOT_SUPPORTED):
			break;
		default: {
				ERROR_BLOCK(LEV_ERROR, "Unknown SecToPri FUNCTION: " << mHeader.GetFuncEnum(), DLERR_UNKNOWN_FUNC);
				return false;
			}
		}

		//now check the fcb, it should always be zero
		if(mHeader.IsFcbSet()) {
			ERROR_BLOCK(LEV_ERROR, "FCB set for SecToPri FUNCTION: " << mHeader.GetFuncEnum(), DLERR_UNEXPECTED_FCB);
			return false;
		}
	}

	return true; //valid!
}

}

