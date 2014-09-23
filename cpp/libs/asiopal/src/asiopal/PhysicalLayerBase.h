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
#ifndef ASIOPAL_PHYSICALLAYERBASE_H
#define ASIOPAL_PHYSICALLAYERBASE_H

#include <openpal/executor/IExecutor.h>
#include <openpal/channel/IPhysicalLayer.h>
#include <openpal/logging/LogRoot.h>

#include <system_error>
#include <assert.h>

namespace asiopal
{

class PLAS_Base;

// This is the base class for the new async physical layers. It assumes that all of the functions
// are called from a single thread.
class PhysicalLayerBase : public openpal::IPhysicalLayer
{
	class State : public  openpal::IChannelState
	{
	public:
		State();

		bool isOpen;
		bool isOpening;
		bool isReading;
		bool isWriting;
		bool isClosing;

		bool IsOpen() const;
		bool IsOpening() const;
		bool IsReading() const;
		bool IsWriting() const;
		bool IsClosing() const;
		bool IsClosed() const;

		bool CanOpen() const;
		bool CanClose() const;
		bool CanRead() const;
		bool CanWrite() const;

		bool CallbacksPending() const;

		bool CheckForClose();
	};

public:
	PhysicalLayerBase(openpal::LogRoot& root, openpal::IExecutor& executor);

	// destructor should only be called once the object is totally finished with all of its async operations
	// to avoid segfaulting. There are a # of asserts that make sure the object has been shutdown properly.
	virtual ~PhysicalLayerBase() {}

	// Implement IChannelState
	bool IsOpen() const
	{
		return state.IsOpen();
	}
	bool IsOpening() const
	{
		return state.IsOpening();
	}
	bool IsReading() const
	{
		return state.IsReading();
	}
	bool IsWriting() const
	{
		return state.IsWriting();
	}
	bool IsClosing() const
	{
		return state.IsClosing();
	}
	bool IsClosed() const
	{
		return state.IsClosed();
	}

	bool CanOpen() const
	{
		return state.CanOpen();
	}
	bool CanClose() const
	{
		return state.CanClose();
	}
	bool CanRead() const
	{
		return state.CanRead();
	}
	bool CanWrite() const
	{
		return state.CanWrite();
	}	

	/* Implement IPhysicalLayer - Events from the outside */
	virtual void BeginOpen() override final;
	virtual void BeginClose() override final;
	virtual void BeginWrite(const  openpal::ReadOnlyBuffer&) override final;
	virtual void BeginRead(openpal::WriteBuffer&) override final;

	// Not an event delegated to the states
	void SetHandler(openpal::IPhysicalLayerCallbacks* apHandler);

	/* Actions taken by the states - These must be implemented by the concrete
	classes inherited from this class */
	virtual void DoOpen() = 0;
	virtual void DoClose() = 0;
	virtual void DoOpeningClose()
	{
		DoClose();    //optionally override this action
	}
	virtual void DoRead(openpal::WriteBuffer&) = 0;
	virtual void DoWrite(const  openpal::ReadOnlyBuffer&) = 0;

	// These can be optionally overriden to do something more interesting, i.e. specific logging
	virtual void DoOpenCallback() {}
	virtual void DoOpenSuccess() {}
	virtual void DoOpenFailure() {}

	void DoWriteSuccess();
	void DoThisLayerDown();
	void DoReadCallback(const  openpal::ReadOnlyBuffer& arBuffer);

	//Error reporting function(s)
	openpal::Logger& GetLogger()
	{
		return logger;
	}

protected:

	//Internally produced events
	void OnOpenCallback(const std::error_code& err);
	void OnReadCallback(const std::error_code& err, uint8_t* pBuffer, uint32_t numRead);
	void OnWriteCallback(const std::error_code& errr, uint32_t  numWritten);
	
	openpal::Logger logger;
	openpal::IExecutor* pExecutor;

	// "user" object that recieves the callbacks
	openpal::IPhysicalLayerCallbacks* pCallbacks;

	// State object that tracks the activities of the class, state pattern too heavy
	PhysicalLayerBase::State state;

private:

	void StartClose();
};

inline void PhysicalLayerBase::SetHandler(openpal::IPhysicalLayerCallbacks* apHandler)
{
	assert(pCallbacks == nullptr);
	assert(apHandler != nullptr);
	this->pCallbacks = apHandler;
}

}
#endif
