
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
#ifndef __PHYSICAL_LAYER_ASYNC_BASE_H_
#define __PHYSICAL_LAYER_ASYNC_BASE_H_

#include <boost/system/error_code.hpp>

#include "IPhysicalLayerAsync.h"
#include "Loggable.h"

namespace opendnp3
{

class PLAS_Base;

// This is the base class for the new async physical layers. It assumes that all of the functions
// are called from a single thread.
class PhysicalLayerAsyncBase : public IPhysicalLayerAsync, public Loggable
{
	class State : public IPhysicalLayerState
	{
	public:
		State();

		bool mOpen;
		bool mOpening;
		bool mReading;
		bool mWriting;
		bool mClosing;

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
		std::string ConvertStateToString() const;

		bool CheckForClose();
	};

public:
	PhysicalLayerAsyncBase(Logger*);

	// destructor should only be called once the object is totally finished with all of its async operations
	// to avoid segfaulting. There are a # of asserts that make sure the object has been shutdown properly.
	virtual ~PhysicalLayerAsyncBase() {}

	/* Implement IPhysicalLayerState */
	bool IsOpen() const {
		return mState.IsOpen();
	}
	bool IsOpening() const {
		return mState.IsOpening();
	}
	bool IsReading() const {
		return mState.IsReading();
	}
	bool IsWriting() const {
		return mState.IsWriting();
	}
	bool IsClosing() const {
		return mState.IsClosing();
	}
	bool IsClosed() const {
		return mState.IsClosed();
	}

	bool CanOpen() const {
		return mState.CanOpen();
	}
	bool CanClose() const {
		return mState.CanClose();
	}
	bool CanRead() const {
		return mState.CanRead();
	}
	bool CanWrite() const {
		return mState.CanWrite();
	}

	std::string ConvertStateToString() const {
		return mState.ConvertStateToString();
	}

	/* Implement IPhysicalLayerAsync - Events from the outside */
	void AsyncOpen();
	void AsyncClose();
	void AsyncWrite(const uint8_t*, size_t);
	void AsyncRead(uint8_t*, size_t);

	// Not an event delegated to the states
	void SetHandler(IHandlerAsync* apHandler);

	/* Actions taken by the states - These must be implemented by the concrete
	classes inherited from this class */
	virtual void DoOpen() = 0;
	virtual void DoClose() = 0;
	virtual void DoOpeningClose() {
		DoClose();    //optionally override this action
	}
	virtual void DoAsyncRead(uint8_t*, size_t) = 0;
	virtual void DoAsyncWrite(const uint8_t*, size_t) = 0;

	// These can be optionally overriden to do something more interesting, i.e. specific logging
	virtual void DoOpenCallback() {}
	virtual void DoOpenSuccess() {}
	virtual void DoOpenFailure() {}

	void DoWriteSuccess();
	void DoThisLayerDown();
	void DoReadCallback(uint8_t*, size_t);

	//Error reporting function(s)
	Logger* GetLogger() {
		return mpLogger;
	}

protected:

	//Internally produced events
	void OnOpenCallback(const boost::system::error_code& arError);
	void OnReadCallback(const boost::system::error_code& arError, uint8_t*, size_t aSize);
	void OnWriteCallback(const boost::system::error_code& arError, size_t aSize);

	// "user" object that recieves the callbacks
	IHandlerAsync* mpHandler;

	// State object that tracks the activities of the class, state pattern too heavy
	PhysicalLayerAsyncBase::State mState;

private:

	void StartClose();
};

inline void PhysicalLayerAsyncBase::SetHandler(IHandlerAsync* apHandler)
{
	assert(mpHandler == NULL);
	assert(apHandler != NULL);
	this->mpHandler = apHandler;
}

}
#endif
