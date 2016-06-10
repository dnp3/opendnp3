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
#include "asiopal/PhysicalLayerBase.h"

#include <openpal/channel/IPhysicalLayerCallbacks.h>

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/LogLevels.h>

#include <sstream>

using namespace std;
using namespace openpal;

namespace asiopal
{

///////////////////////////////////
// State object
///////////////////////////////////

PhysicalLayerBase::State::State() :
	isOpen(false),
	isOpening(false),
	isReading(false),
	isWriting(false),
	isClosing(false)
{}

bool PhysicalLayerBase::State::IsOpen() const
{
	return isOpen;
}

bool PhysicalLayerBase::State::IsOpening() const
{
	return isOpening;
}

bool PhysicalLayerBase::State::IsReading() const
{
	return isReading;
}

bool PhysicalLayerBase::State::IsWriting() const
{
	return isWriting;
}

bool PhysicalLayerBase::State::IsClosing() const
{
	return isClosing;
}

bool PhysicalLayerBase::State::IsClosed() const
{
	return !(isOpening || isOpen || isClosing || isReading || isWriting);
}

bool PhysicalLayerBase::State::CanOpen() const
{
	return this->IsClosed();
}

bool PhysicalLayerBase::State::CanClose() const
{
	return (isOpen || isOpening) && !isClosing;
}

bool PhysicalLayerBase::State::CanRead() const
{
	return isOpen && !isClosing && !isReading;
}

bool PhysicalLayerBase::State::CanWrite() const
{
	return isOpen && !isClosing && !isWriting;
}

bool PhysicalLayerBase::State::CallbacksPending() const
{
	return isOpening || isReading || isWriting;
}

bool PhysicalLayerBase::State::CheckForClose()
{
	if (isClosing && !this->CallbacksPending())
	{
		isClosing = isOpen = false;
		return true;
	}
	else return false;
}

///////////////////////////////////
// PhysicalLayerBase
///////////////////////////////////

PhysicalLayerBase::PhysicalLayerBase(openpal::Logger logger_) :
	logger(logger_),
	pExecutor(nullptr),
	pCallbacks(nullptr)
{

}

void PhysicalLayerBase::SetExecutor(IExecutor& executor)
{
	pExecutor = &executor;
}

////////////////////////////////////
// External Events
////////////////////////////////////

void PhysicalLayerBase::BeginOpen()
{
	if(state.CanOpen())
	{
		state.isOpening = true;
		this->DoOpen();
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
	}
}

/** Marshalls the DoThisLayerDown call
so that all upward OnLowerLayerDown calls are made directly
from the io_service.
*/
void PhysicalLayerBase::BeginClose()
{
	this->StartClose();
	if (state.CheckForClose())
	{
		this->DoThisLayerDown();
	}
}

void PhysicalLayerBase::StartClose()
{
	if(!state.IsClosing())
	{
		if(state.CanClose())
		{
			if (pChannelStatistics)
			{
				++pChannelStatistics->numClose;
			}

			state.isClosing = true;

			if (state.isOpening) this->DoOpeningClose();
			else this->DoClose();
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
		}
	}
}

void PhysicalLayerBase::BeginWrite(const openpal::RSlice& buffer)
{
	if (state.CanWrite())
	{
		if (buffer.Size() > 0)
		{
			state.isWriting = true;
			this->DoWrite(buffer);
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Client wrote a length of 0");
			auto callback = [this]()
			{
				this->DoWriteSuccess();
			};
			pExecutor->PostLambda(callback);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
	}
}

void PhysicalLayerBase::BeginRead(WSlice& buffer)
{
	if(state.CanRead())
	{
		if (buffer.Size() > 0)
		{
			state.isReading = true;
			this->DoRead(buffer);
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Client read a length of 0");
			auto callback = [this, buffer]()
			{
				this->DoReadCallback(RSlice());
			};
			pExecutor->PostLambda(callback);
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
	}
}

///////////////////////////////////////
// Internal events
///////////////////////////////////////

void PhysicalLayerBase::OnOpenCallback(const std::error_code& err)
{
	if (state.isOpening)
	{
		state.isOpening = false;

		this->DoOpenCallback();

		if(err)
		{
			FORMAT_LOG_BLOCK(logger, logflags::WARN, "error: %s", err.message().c_str());

			if (pChannelStatistics)
			{
				++pChannelStatistics->numOpenFail;
			}

			state.CheckForClose();
			this->DoOpenFailure();
			if (pCallbacks)
			{
				pCallbacks->OnOpenFailure();
			}
		}
		else   // successful connection
		{
			if(this->IsClosing())   // but the connection was closed
			{
				state.CheckForClose();
				this->DoClose();
				if (pCallbacks)
				{
					pCallbacks->OnOpenFailure();
				}
			}
			else
			{
				if (pChannelStatistics)
				{
					++pChannelStatistics->numOpen;
				}

				state.isOpen = true;
				this->DoOpenSuccess();
				if (pCallbacks)
				{
					pCallbacks->OnLowerLayerUp();
				}
			}
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
	}
}

void PhysicalLayerBase::OnReadCallback(const std::error_code& err, uint8_t* pBuffer, uint32_t numRead)
{
	if (state.isReading)
	{
		state.isReading = false;

		if(err)
		{
			SIMPLE_LOG_BLOCK(logger, logflags::WARN, err.message().c_str());
			if(state.CanClose()) this->StartClose();
		}
		else
		{
			if (pChannelStatistics)
			{
				pChannelStatistics->numBytesRx += numRead;
			}

			if (!state.isClosing)
			{
				RSlice buffer(pBuffer, numRead);
				this->DoReadCallback(buffer);
			}
		}

		if(state.CheckForClose()) this->DoThisLayerDown();
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
	}
}

void PhysicalLayerBase::OnWriteCallback(const std::error_code& err, uint32_t numWritten)
{
	if (state.isWriting)
	{
		state.isWriting = false;

		if(err)
		{
			SIMPLE_LOG_BLOCK(logger, logflags::WARN, err.message().c_str());
			if(state.CanClose()) this->StartClose();
		}
		else
		{
			if (pChannelStatistics)
			{
				pChannelStatistics->numBytesTx += numWritten;
			}

			if (!state.isClosing)
			{
				this->DoWriteSuccess();
			}
		}

		if(state.CheckForClose()) this->DoThisLayerDown();
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, logflags::ERR, "Invalid operation for state");
	}
}

////////////////////////////////////
// Actions
////////////////////////////////////

void PhysicalLayerBase::DoWriteSuccess()
{
	if (pCallbacks)
	{
		pCallbacks->OnSendResult(true);
	}
}

void PhysicalLayerBase::DoThisLayerDown()
{
	if (pCallbacks)
	{
		pCallbacks->OnLowerLayerDown();
	}
}

void PhysicalLayerBase::DoReadCallback(const RSlice& arBuffer)
{
	if (pCallbacks)
	{
		pCallbacks->OnReceive(arBuffer);
	}
}

} //end namespace

