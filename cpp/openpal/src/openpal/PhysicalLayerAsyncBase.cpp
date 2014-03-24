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
#include "PhysicalLayerAsyncBase.h"

#include "IHandlerAsync.h"
#include "LoggableMacros.h"
#include "IExecutor.h"
#include "LogLevels.h"

#include <sstream>

using namespace std;

namespace openpal
{

///////////////////////////////////
// State object
///////////////////////////////////

PhysicalLayerAsyncBase::State::State() :
	mOpen(false),
	mOpening(false),
	mReading(false),
	mWriting(false),
	mClosing(false)
{}

bool PhysicalLayerAsyncBase::State::IsOpen() const
{
	return mOpen;
}

bool PhysicalLayerAsyncBase::State::IsOpening() const
{
	return mOpening;
}

bool PhysicalLayerAsyncBase::State::IsReading() const
{
	return mReading;
}

bool PhysicalLayerAsyncBase::State::IsWriting() const
{
	return mWriting;
}

bool PhysicalLayerAsyncBase::State::IsClosing() const
{
	return mClosing;
}

bool PhysicalLayerAsyncBase::State::IsClosed() const
{
	return !(mOpening || mOpen || mClosing || mReading || mWriting);
}

bool PhysicalLayerAsyncBase::State::CanOpen() const
{
	return this->IsClosed();
}

bool PhysicalLayerAsyncBase::State::CanClose() const
{
	return (mOpen || mOpening) && !mClosing;
}

bool PhysicalLayerAsyncBase::State::CanRead() const
{
	return mOpen && !mClosing && !mReading;
}

bool PhysicalLayerAsyncBase::State::CanWrite() const
{
	return mOpen && !mClosing && !mWriting;
}

bool PhysicalLayerAsyncBase::State::CallbacksPending() const
{
	return mOpening || mReading || mWriting;
}

bool PhysicalLayerAsyncBase::State::CheckForClose()
{
	if(mClosing && !this->CallbacksPending())
	{
		mClosing = mOpen = false;
		return true;
	}
	else return false;
}

std::string PhysicalLayerAsyncBase::State::ConvertStateToString() const
{
	std::ostringstream oss;
	oss << "Open: " << mOpen << " Opening: " << mOpening
	    << " Reading: " << mReading << " Writing: " << mWriting
	    << " Closing: " << mClosing;

	return oss.str();
}

///////////////////////////////////
// PhysicalLayerAsyncBase
///////////////////////////////////

PhysicalLayerAsyncBase::PhysicalLayerAsyncBase(const LogConfig& config) :
	logRoot(config.GetLog(), config.GetFilters()),
	Loggable(logRoot.GetLogger(config.GetRootId())),
	mpHandler(nullptr)
{

}

////////////////////////////////////
// External Events
////////////////////////////////////

void PhysicalLayerAsyncBase::AsyncOpen()
{
	if(state.CanOpen())
	{
		state.mOpening = true;
		this->DoOpen();
	}
	else
	{
		LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
	}
}

/** Marshalls the DoThisLayerDown call
so that all upward OnLowerLayerDown calls are made directly
from the io_service.
*/
void PhysicalLayerAsyncBase::AsyncClose()
{
	this->StartClose();
	if (state.CheckForClose())
	{
		this->DoThisLayerDown();
	}
}

void PhysicalLayerAsyncBase::StartClose()
{
	if(!state.IsClosing())   //TODO - kind of hack as it deviates from the current model.
	{
		if(state.CanClose())
		{
			state.mClosing = true;

			if(state.mOpening) this->DoOpeningClose();
			else this->DoClose();
		}
		else
		{
			LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
		}
	}
}

void PhysicalLayerAsyncBase::AsyncWrite(const openpal::ReadOnlyBuffer& arBuffer)
{
	if (state.CanWrite())
	{
		if (arBuffer.Size() > 0)
		{
			state.mWriting = true;
			this->DoAsyncWrite(arBuffer);
		}
		else
		{
			LOG_BLOCK(log::ERR, "Client wrote a length of 0");
			this->GetExecutor()->Post([this]()
			{
				this->DoWriteSuccess();
			});
		}
	}
	else
	{
		LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
	}
}

void PhysicalLayerAsyncBase::AsyncRead(WriteBuffer& arBuffer)
{
	if(state.CanRead())
	{
		if (arBuffer.Size() > 0)
		{
			state.mReading = true;
			this->DoAsyncRead(arBuffer);
		}
		else
		{
			LOG_BLOCK(log::ERR, "Client read a length of 0");
			this->GetExecutor()->Post([this, arBuffer]()
			{
				this->DoReadCallback(ReadOnlyBuffer());
			});
		}
	}
	else
	{
		LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
	}
}

///////////////////////////////////////
// Internal events
///////////////////////////////////////

void PhysicalLayerAsyncBase::OnOpenCallback(const std::error_code& arErr)
{
	if(state.mOpening)
	{
		state.mOpening = false;

		this->DoOpenCallback();

		if(arErr)
		{
			LOG_BLOCK(log::WARN, arErr.message());
			state.CheckForClose();
			this->DoOpenFailure();
			if(mpHandler) mpHandler->OnOpenFailure();
		}
		else   // successful connection
		{
			if(this->IsClosing())   // but the connection was closed
			{
				state.CheckForClose();
				this->DoClose();
				if(mpHandler) mpHandler->OnOpenFailure();
			}
			else
			{
				state.mOpen = true;
				this->DoOpenSuccess();
				if(mpHandler) mpHandler->OnLowerLayerUp();
			}
		}
	}
	else
	{
		LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
	}
}

void PhysicalLayerAsyncBase::OnReadCallback(const std::error_code& arErr, uint8_t* apBuffer, uint32_t aNumRead)
{
	if(state.mReading)
	{
		state.mReading = false;

		if(arErr)
		{
			LOG_BLOCK(log::WARN, arErr.message());
			if(state.CanClose()) this->StartClose();
		}
		else
		{
			if(state.mClosing)
			{
				LOG_BLOCK(log::DEBUG, "Ignoring received bytes since layer is closing: " << aNumRead);
			}
			else
			{
				ReadOnlyBuffer buffer(apBuffer, aNumRead);
				this->DoReadCallback(buffer);
			}
		}

		if(state.CheckForClose()) this->DoThisLayerDown();
	}
	else
	{
		LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
	}
}

void PhysicalLayerAsyncBase::OnWriteCallback(const std::error_code& arErr, uint32_t  aNumBytes)
{
	if(state.mWriting)
	{
		state.mWriting = false;

		if(arErr)
		{
			LOG_BLOCK(log::WARN, arErr.message());
			if(state.CanClose()) this->StartClose();
		}
		else
		{
			if(state.mClosing)
			{
				LOG_BLOCK(log::DEBUG, "Ignoring written bytes since layer is closing: " << aNumBytes);
			}
			else
			{
				this->DoWriteSuccess();
			}
		}

		if(state.CheckForClose()) this->DoThisLayerDown();
	}
	else
	{
		LOG_BLOCK(log::ERR, "Invalid operation for state: " << this->ConvertStateToString());
	}
}

////////////////////////////////////
// Actions
////////////////////////////////////

void PhysicalLayerAsyncBase::DoWriteSuccess()
{
	if (mpHandler)
	{
		mpHandler->OnSendResult(true);
	}
}

void PhysicalLayerAsyncBase::DoThisLayerDown()
{
	if (mpHandler)
	{
		mpHandler->OnLowerLayerDown();
	}
}

void PhysicalLayerAsyncBase::DoReadCallback(const ReadOnlyBuffer& arBuffer)
{
	if (mpHandler)
	{
		mpHandler->OnReceive(arBuffer);
	}
}

} //end namespace

