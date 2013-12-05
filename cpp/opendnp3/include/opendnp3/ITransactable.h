#ifndef __I_TRANSACTABLE_H_
#define	__I_TRANSACTABLE_H_

#include <assert.h>
#include <stddef.h>

namespace opendnp3
{

/**
    A Transaction is a group of point Updates that occur together.
	This is necessary because often times a single event will cause many values
	to change and that should be handled as a group so the data isn't unecessarily
	inconsistant. One example is a "master alarm" flag that is a summary of specific alarms,
	if we couldn't group the updates toegether we might see the master alarm flag turn on
	before any of the specific alarms had activated. Grouping the updates together also leads
	to more effecient communications because large chunks of data get transmitted at a time and
	we don't get into the pathelogical case of sending one update at a time.

	\section Usage
	\code
	  IDataObserver* pDataObserver; // usually kept as a member on a class
	  Transaction tr(pDataObserver);
	  Analog a;
	  a.SetValue(33);
	  a.SetToNow();
	  a.SetQuality(AQ_ONLINE);
	  pDataObserver.Update(a, 2);
	\endcode
*/
class ITransactable
{
	friend class Transaction;

public:

	ITransactable() : mInProgress(false) {}
	virtual ~ITransactable() {}

	// Enfore pre/post conditions on _Start/_End Operation
	void Start();
	void End();

protected:

	bool InProgress();

	// concrete classes will define these functions
	virtual void _Start() = 0;
	virtual void _End() = 0;

private:

	bool mInProgress;

};

inline bool ITransactable::InProgress()
{
	return mInProgress;
}

inline void ITransactable::Start()
{
	this->_Start();
	assert(!mInProgress);
	mInProgress = true;
}

inline void ITransactable::End()
{
	assert(mInProgress);
	mInProgress = false;
	this->_End();
}



/**
  This is a helper class that handles the starting and ending of the transaction
  in an exeception safe manner. If we manually call Start, do some updates and
  have an exception then its possible we wont end the transaction. By using this
  transaction object the stack unwinding will guarentee that the transaction is
  correctly cleaned up.
*/
class Transaction
{
public:
	Transaction(ITransactable& arTransactable)
		: mpTransactable(&arTransactable), mIsEnded(false) {
		mpTransactable->Start();
	}

	Transaction(ITransactable* apTransactable)
		: mpTransactable(apTransactable), mIsEnded(false) {
		mpTransactable->Start();
	}

	Transaction()
		: mpTransactable(nullptr), mIsEnded(false) {

	}

	void Start(ITransactable* apTransactable) {
		assert(mpTransactable == nullptr); assert(!mIsEnded);
		mpTransactable = apTransactable;
		mpTransactable->Start();
	}

	void End() {
		assert(mpTransactable != nullptr); assert(!mIsEnded);
		mIsEnded = true;
		mpTransactable->End();
	}

	~Transaction() {
		if(mpTransactable != nullptr && !mIsEnded) mpTransactable->End();
	}

private:
	ITransactable* mpTransactable;
	bool mIsEnded;
};

}

#endif
