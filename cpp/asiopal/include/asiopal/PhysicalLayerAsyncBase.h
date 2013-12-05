#ifndef __PHYSICAL_LAYER_ASYNC_BASE_H_
#define __PHYSICAL_LAYER_ASYNC_BASE_H_

#include <boost/system/error_code.hpp>

#include <openpal/Visibility.h>
#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/Loggable.h>
#include <openpal/Location.h>

namespace asiopal
{

class PLAS_Base;

// This is the base class for the new async physical layers. It assumes that all of the functions
// are called from a single thread.
class DLL_LOCAL PhysicalLayerAsyncBase : public openpal::IPhysicalLayerAsync, public openpal::Loggable
{
	class State : public openpal::IChannelState
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
#ifndef OPENDNP3_STRIP_LOG_MESSAGES
		std::string ConvertStateToString() const;
#endif

		bool CheckForClose();
	};

public:
	PhysicalLayerAsyncBase(openpal::Logger&);

	// destructor should only be called once the object is totally finished with all of its async operations
	// to avoid segfaulting. There are a # of asserts that make sure the object has been shutdown properly.
	virtual ~PhysicalLayerAsyncBase() {}

	/* Implement IChannelState */
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

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	std::string ConvertStateToString() const {
		return mState.ConvertStateToString();
	}
#endif

	/* Implement IPhysicalLayerAsync - Events from the outside */
	void AsyncOpen();
	void AsyncClose();
	void AsyncWrite(const openpal::ReadOnlyBuffer&);
	void AsyncRead(openpal::WriteBuffer&);

	// Not an event delegated to the states
	void SetHandler(openpal::IHandlerAsync* apHandler);

	/* Actions taken by the states - These must be implemented by the concrete
	classes inherited from this class */
	virtual void DoOpen() = 0;
	virtual void DoClose() = 0;
	virtual void DoOpeningClose() {
		DoClose();    //optionally override this action
	}
	virtual void DoAsyncRead(openpal::WriteBuffer&) = 0;
	virtual void DoAsyncWrite(const openpal::ReadOnlyBuffer&) = 0;

	// These can be optionally overriden to do something more interesting, i.e. specific logging
	virtual void DoOpenCallback() {}
	virtual void DoOpenSuccess() {}
	virtual void DoOpenFailure() {}

	void DoWriteSuccess();
	void DoThisLayerDown();
	void DoReadCallback(const openpal::ReadOnlyBuffer& arBuffer);

	//Error reporting function(s)
	openpal::Logger& GetLogger() {
		return mLogger;
	}

protected:

	//Internally produced events
	void OnOpenCallback(const boost::system::error_code& arError);
	void OnReadCallback(const boost::system::error_code& arError, uint8_t* apBuffer, size_t aNumRead);
	void OnWriteCallback(const boost::system::error_code& arError, size_t aNumBytes);

	// "user" object that recieves the callbacks
	openpal::IHandlerAsync* mpHandler;

	// State object that tracks the activities of the class, state pattern too heavy
	PhysicalLayerAsyncBase::State mState;

private:

	void StartClose();
};

inline void PhysicalLayerAsyncBase::SetHandler(openpal::IHandlerAsync* apHandler)
{
	assert(mpHandler == nullptr);
	assert(apHandler != nullptr);
	this->mpHandler = apHandler;
}

}
#endif
