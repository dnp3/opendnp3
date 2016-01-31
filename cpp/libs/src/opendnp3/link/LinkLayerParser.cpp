/*
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
#include "LinkLayerParser.h"

#include "opendnp3/link/CRC.h"
#include "opendnp3/link/IFrameSink.h"

#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

LinkLayerParser::LinkLayerParser(const Logger& logger_, LinkChannelStatistics* pStatistics_) :
	logger(logger_),
	pStatistics(pStatistics_),
	state(State::FindSync),
	frameSize(0),
	buffer(rxBuffer, LPDU_MAX_FRAME_SIZE)
{

}

void LinkLayerParser::Reset()
{
	state = State::FindSync;
	frameSize = 0;
	buffer.Reset();
}

WSlice LinkLayerParser::WriteBuff() const
{
	return WSlice(buffer.WriteBuff(), buffer.NumWriteBytes());
}

void LinkLayerParser::OnRead(uint32_t numBytes, IFrameSink& sink)
{
	buffer.AdvanceWrite(numBytes);

	while (ParseUntilComplete() == State::Complete)
	{
		if (pStatistics)
		{
			++pStatistics->numLinkFrameRx;
		}
		this->PushFrame(sink);
		state = State::FindSync;
	}

	buffer.Shift();
}

LinkLayerParser::State LinkLayerParser::ParseUntilComplete()
{
	auto lastState = this->state;
	// continue as long as we're making progress, i.e. a state change
	while ((this->state = ParseOneStep()) != lastState)
	{
		lastState = state;
	}
	return state;
}

LinkLayerParser::State LinkLayerParser::ParseOneStep()
{
	switch (state)
	{
	case(State::FindSync) :
		return ParseSync();
	case(State::ReadHeader) :
		return ParseHeader();
	case(State::ReadBody) :
		return ParseBody();
	default:
		return state;
	}
}

LinkLayerParser::State LinkLayerParser::ParseSync()
{
	if (this->buffer.NumBytesRead() >= 10 && buffer.Sync())
	{
		return State::ReadHeader;
	}
	else
	{
		return State::FindSync;
	}
}

LinkLayerParser::State LinkLayerParser::ParseHeader()
{
	if (this->buffer.NumBytesRead() >= 10)
	{
		if (this->ReadHeader())
		{
			return State::ReadBody;
		}
		else
		{
			this->FailFrame();
			return State::FindSync;
		}
	}
	else
	{
		return State::ReadHeader;
	}
}

LinkLayerParser::State LinkLayerParser::ParseBody()
{
	if (buffer.NumBytesRead() < this->frameSize)
	{
		return State::ReadBody;
	}
	else
	{
		if(this->ValidateBody())
		{
			this->TransferUserData();
			return State::Complete;
		}
		else
		{
			this->FailFrame();
			return State::FindSync;
		}
	}
}



void LinkLayerParser::PushFrame(IFrameSink& sink)
{
	LinkHeaderFields fields(
	    header.GetFuncEnum(),
	    header.IsFromMaster(),
	    header.IsFcbSet(),
	    header.IsFcvDfcSet(),
	    header.GetDest(),
	    header.GetSrc()
	);

	sink.OnFrame(fields, userData);

	buffer.AdvanceRead(frameSize);
}

void LinkLayerParser::TransferUserData()
{
	uint32_t len = header.GetLength() - LPDU_MIN_LENGTH;
	LinkFrame::ReadUserData(buffer.ReadBuffer() + LPDU_HEADER_SIZE, rxBuffer, len);
	userData = RSlice(rxBuffer, len);
}

bool LinkLayerParser::ReadHeader()
{
	header.Read(buffer.ReadBuffer());
	if (CRC::IsCorrectCRC(buffer.ReadBuffer(), LI_CRC))
	{
		if (ValidateHeaderParameters())
		{
			return true;
		}
		else
		{
			if (pStatistics) ++pStatistics->numBadLinkFrameRx;
			return false;
		}
	}
	else
	{
		if (pStatistics) ++pStatistics->numCrcError;
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_CRC, "CRC failure in header");
		return false;
	}
}

bool LinkLayerParser::ValidateBody()
{
	uint32_t len = header.GetLength() - LPDU_MIN_LENGTH;
	if (LinkFrame::ValidateBodyCRC(buffer.ReadBuffer() + LPDU_HEADER_SIZE, len))
	{
		FORMAT_LOG_BLOCK(logger, flags::LINK_RX,
		                 "Function: %s Dest: %u Source: %u Length: %u",
		                 LinkFunctionToString(header.GetFuncEnum()),
		                 header.GetDest(),
		                 header.GetSrc(),
		                 header.GetLength());

		FORMAT_HEX_BLOCK(logger, flags::LINK_RX_HEX, buffer.ReadBuffer().Take(frameSize), 10, 18);

		return true;
	}
	else
	{
		SIMPLE_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_CRC, "CRC failure in body");
		if (pStatistics)
		{
			++pStatistics->numCrcError;
		}
		return false;
	}
}

bool LinkLayerParser::ValidateHeaderParameters()
{
	if(!header.ValidLength())
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_INVALID_LENGTH, "LENGTH out of range [5,255]: %i", header.GetLength());
		return false;
	}

	// some combinations of these header parameters are invalid
	// check for them here

	//Now make sure that the function code is known and that the FCV is appropriate
	if (!this->ValidateFunctionCode())
	{
		return false;
	}

	uint8_t user_data_length = header.GetLength() - LPDU_MIN_LENGTH;
	frameSize = LinkFrame::CalcFrameSize(user_data_length);
	LinkFunction func = header.GetFuncEnum();

	// make sure that the presence/absence of user data
	// matches the function code
	if(func == LinkFunction::PRI_CONFIRMED_USER_DATA || func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
	{
		if(user_data_length > 0)
		{
			frameSize = LinkFrame::CalcFrameSize(user_data_length);
		}
		else
		{
			FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_NO_DATA, "User data with no payload. FUNCTION: %s", LinkFunctionToString(func));
			return false;
		}
	}
	else
	{
		if(user_data_length > 0)
		{
			FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_UNEXPECTED_DATA, "Unexpected LENGTH in frame: %i with FUNCTION: %s", user_data_length, LinkFunctionToString(func));
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
			FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_UNEXPECTED_DATA, "Unexpected LENGTH in frame: %i with FUNCTION: %s", user_data_length, LinkFunctionToString(func));
			return false;
		}
	}
	else
	{
		if(func == LinkFunction::PRI_CONFIRMED_USER_DATA || func == LinkFunction::PRI_UNCONFIRMED_USER_DATA)
		{
			FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_NO_DATA, "User data packet received with zero payload. FUNCTION: %s", LinkFunctionToString(func));
			return false;
		}
	}

	return true;
}

void LinkLayerParser::FailFrame()
{
	// All you have to do is advance the reader by one, when the resync happens the data will disappear
	buffer.AdvanceRead(1);
}

bool LinkLayerParser::ValidateFunctionCode()
{
	//Now make sure that the function code is known and that the FCV is appropriate
	if(header.IsPriToSec())
	{
		bool fcv_set = false;

		switch(header.GetFuncEnum())
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
				FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_FUNC, "Unknown PriToSec FUNCTION: %s", LinkFunctionToString(header.GetFuncEnum()));
				return false;
			}
		}

		//now check the fcv
		if(fcv_set != header.IsFcvDfcSet())
		{
			FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNEXPECTED_FCV, "Bad FCV for FUNCTION: %s", LinkFunctionToString(header.GetFuncEnum()));
			return false;
		}

		//if fcv isn't expected to be set, fcb can be either 1 or 0, doesn't matter

	}
	else   // SecToPri - just validate the function codes and that FCB is 0
	{
		switch(header.GetFuncEnum())
		{
		case(LinkFunction::SEC_ACK):
		case(LinkFunction::SEC_NACK):
		case(LinkFunction::SEC_LINK_STATUS):
		case(LinkFunction::SEC_NOT_SUPPORTED):
			break;
		default:
			{
				FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_UNKNOWN_FUNC, "Unknown SecToPri FUNCTION: %s", LinkFunctionToString(header.GetFuncEnum()));
				return false;
			}
		}

		//now check the fcb, it should always be zero
		if(header.IsFcbSet())
		{
			FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::ERR, DLERR_UNEXPECTED_FCB, "FCB set for SecToPri FUNCTION: %s", LinkFunctionToString(header.GetFuncEnum()));
			return false;
		}
	}

	return true; //valid!
}

}

