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
#include <assert.h>

#include "opendnp3/link/DNPCrc.h"
#include "opendnp3/link/LinkReceiverStates.h"
#include "opendnp3/link/IFrameSink.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

const uint8_t LinkLayerReceiver::M_SYNC_PATTERN[2] = {0x05, 0x64};

LinkLayerReceiver::LinkLayerReceiver(Logger aLogger, IFrameSink* apSink) :
	Loggable(aLogger),
	mFrameSize(0),
	mpSink(apSink),
	mpState(LRS_Sync::Inst()),
	receiverBuffer(),
	mBuffer(receiverBuffer.Buffer(), receiverBuffer.Size())
{

}

void LinkLayerReceiver::Reset()
{
	mFrameSize = 0;
	mpState = LRS_Sync::Inst();
	mBuffer.Reset();
}

WriteBuffer LinkLayerReceiver::WriteBuff() const
{
	WriteBuffer b(mBuffer.WriteBuff(), mBuffer.NumWriteBytes());
	return b;
}

void LinkLayerReceiver::OnRead(uint32_t aNumBytes)
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
	switch(mHeader.GetFuncEnum())
	{
	case(LinkFunction::PRI_RESET_LINK_STATES):
		mpSink->ResetLinkStates(mHeader.IsFromMaster(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(LinkFunction::PRI_TEST_LINK_STATES):
		mpSink->TestLinkStatus(mHeader.IsFromMaster(), mHeader.IsFcbSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(LinkFunction::PRI_CONFIRMED_USER_DATA):
		mpSink->ConfirmedUserData(mHeader.IsFromMaster(), mHeader.IsFcbSet(), mHeader.GetDest(), mHeader.GetSrc(), TransferUserData());
		break;
	case(LinkFunction::PRI_UNCONFIRMED_USER_DATA):
		mpSink->UnconfirmedUserData(mHeader.IsFromMaster(), mHeader.GetDest(), mHeader.GetSrc(), TransferUserData());
		break;
	case(LinkFunction::PRI_REQUEST_LINK_STATUS):
		mpSink->RequestLinkStatus(mHeader.IsFromMaster(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(LinkFunction::SEC_ACK):
		mpSink->Ack(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(LinkFunction::SEC_NACK):
		mpSink->Nack(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(LinkFunction::SEC_LINK_STATUS):
		mpSink->LinkStatus(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	case(LinkFunction::SEC_NOT_SUPPORTED):
		mpSink->NotSupported(mHeader.IsFromMaster(), mHeader.IsFcvDfcSet(), mHeader.GetDest(), mHeader.GetSrc());
		break;
	default:
		break;
	}

	mBuffer.AdvanceRead(mFrameSize);
}

ReadOnlyBuffer LinkLayerReceiver::TransferUserData()
{
	uint32_t len = mHeader.GetLength() - LS_MIN_LENGTH;
	LinkFrame::ReadUserData(mBuffer.ReadBuff() + LS_HEADER_SIZE, mpUserData, len);
	return ReadOnlyBuffer(mpUserData, len);
}

bool LinkLayerReceiver::ReadHeader()
{
	mHeader.Read(mBuffer.ReadBuff());
	return this->ValidateHeader();
}

bool LinkLayerReceiver::ValidateBody()
{
	uint32_t len = mHeader.GetLength() - LS_MIN_LENGTH;
	if(LinkFrame::ValidateBodyCRC(mBuffer.ReadBuff() + LS_HEADER_SIZE, len)) return true;
	else
	{
		ERROR_BLOCK(LogLevel::Error, "CRC failure in body", DLERR_CRC);
		return false;
	}
}

bool LinkLayerReceiver::ValidateHeader()
{
	//first thing to do is check the CRC
	if(!DNPCrc::IsCorrectCRC(mBuffer.ReadBuff(), LI_CRC))
	{
		ERROR_BLOCK(LogLevel::Error, "CRC failure in header", DLERR_CRC);
		return false;
	}

	if(!mHeader.ValidLength())
	{
		ERROR_BLOCK(LogLevel::Error, "LENGTH out of range [5,255]: " << static_cast<int>(mHeader.GetLength()), DLERR_INVALID_LENGTH);
		return false;
	}

	LOG_BLOCK(LogLevel::Interpret, "<~ " << mHeader.ToString());

	// some combinations of these header parameters are invalid
	// check for them here

	//Now make sure that the function code is known and that the FCV is appropriate
	if(!this->ValidateFunctionCode()) return false;

	uint8_t user_data_length = mHeader.GetLength() - LS_MIN_LENGTH;
	mFrameSize = LinkFrame::CalcFrameSize(user_data_length);
	LinkFunction func = mHeader.GetFuncEnum();

	// make sure that the presence/absence of user data
	// matches the function code
	if(func == LinkFunction::PRI_CONFIRMED_USER_DATA || func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
	{
		if(user_data_length > 0)
		{
			//mFrameSize = LinkFrame::CalcFrameSize(user_data_length);
		}
		else
		{
			ERROR_BLOCK(LogLevel::Error, "User data packet received with zero payload. FUNCTION: " << LinkFunctionToString(func), DLERR_NO_DATA);
			return false;
		}
	}
	else
	{
		if(user_data_length > 0)
		{
			ERROR_BLOCK(LogLevel::Error, "Unexpected LENGTH in frame: " << static_cast<int>(user_data_length) << " with FUNCTION: " << LinkFunctionToString(func), DLERR_UNEXPECTED_DATA);
			return false;
		}

	}

	if(user_data_length > 0)
	{
		if(func == LinkFunction::PRI_CONFIRMED_USER_DATA || func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
		{

		}
		else
		{
			ERROR_BLOCK(LogLevel::Error, "Unexpected LENGTH in frame: " << static_cast<int>(user_data_length) << " with FUNCTION: " << LinkFunctionToString(func), DLERR_UNEXPECTED_DATA);
			return false;
		}
	}
	else
	{
		if(func == LinkFunction::PRI_CONFIRMED_USER_DATA || func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
		{
			ERROR_BLOCK(LogLevel::Error, "User data packet received with zero payload. FUNCTION: " << LinkFunctionToString(func), DLERR_NO_DATA);
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
	if(mHeader.IsPriToSec())
	{
		bool fcv_set = false;

		switch(mHeader.GetFuncEnum())
		{
		case(LinkFunction::PRI_CONFIRMED_USER_DATA):
		case(LinkFunction::PRI_TEST_LINK_STATES):
			fcv_set = true;
			break;
		case(LinkFunction::PRI_REQUEST_LINK_STATUS):
		case(LinkFunction::PRI_RESET_LINK_STATES):
		case(LinkFunction::PRI_UNCONFIRMED_USER_DATA):
			break;
		default:
			{
				ERROR_BLOCK(LogLevel::Warning, "Unknown PriToSec FUNCTION: " << LinkFunctionToString(mHeader.GetFuncEnum()), DLERR_UNKNOWN_FUNC);
				return false;
			}
		}

		//now check the fcv
		if(fcv_set != mHeader.IsFcvDfcSet())
		{
			ERROR_BLOCK(LogLevel::Warning, "Bad FCV for FUNCTION: " << LinkFunctionToString(mHeader.GetFuncEnum()), DLERR_UNEXPECTED_FCV);
			return false;
		}

		//if fcv isn't expected to be set, fcb can be either 1 or 0, doesn't matter

	}
	else   // SecToPri - just validate the function codes and that FCB is 0
	{
		switch(mHeader.GetFuncEnum())
		{
		case(LinkFunction::SEC_ACK):
		case(LinkFunction::SEC_NACK):
		case(LinkFunction::SEC_LINK_STATUS):
		case(LinkFunction::SEC_NOT_SUPPORTED):
			break;
		default:
			{
				ERROR_BLOCK(LogLevel::Error, "Unknown SecToPri FUNCTION: " << LinkFunctionToString(mHeader.GetFuncEnum()), DLERR_UNKNOWN_FUNC);
				return false;
			}
		}

		//now check the fcb, it should always be zero
		if(mHeader.IsFcbSet())
		{
			ERROR_BLOCK(LogLevel::Error, "FCB set for SecToPri FUNCTION: " << LinkFunctionToString(mHeader.GetFuncEnum()), DLERR_UNEXPECTED_FCB);
			return false;
		}
	}

	return true; //valid!
}

}

