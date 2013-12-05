#include "ComparingDataObserver.h"

using namespace openpal;

namespace opendnp3
{

ComparingDataObserver::ComparingDataObserver(FlexibleDataObserver* apObserver) :
	mSameData(false),
	mpObserver(apObserver)
{

}

void ComparingDataObserver::Reset()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mBinaryMap.clear();
	mAnalogMap.clear();
	mCounterMap.clear();
	mControlStatusMap.clear();
	mSetpointStatusMap.clear();
	mSameData = false;
}

bool ComparingDataObserver::IsSameData()
{
	size_t required = mpObserver->mBinaryMap.size() +
	                  mpObserver->mAnalogMap.size() +
	                  mpObserver->mCounterMap.size();

	size_t actual = mBinaryMap.size() +
	                mAnalogMap.size() +
	                mCounterMap.size();

	return (required == actual);

}

bool ComparingDataObserver::WaitForSameData(TimeDuration aWaitMs)
{
	std::unique_lock<std::mutex> lock(mMutex);
	if(!mSameData) mCondition.wait_for(lock, std::chrono::milliseconds(aWaitMs.GetMilliseconds()));
	return mSameData;
}

void ComparingDataObserver::DescribeMissingData()
{
	this->DescribeAny<Binary>(mpObserver->mBinaryMap, mBinaryMap);
	this->DescribeAny<Analog>(mpObserver->mAnalogMap, mAnalogMap);
	this->DescribeAny<Counter>(mpObserver->mCounterMap, mCounterMap);
}

void ComparingDataObserver::Load(const IMeasurementUpdate& arUpdate)
{
	arUpdate.Apply(this);
}

void ComparingDataObserver::_Start()
{
	mMutex.lock();
}

void ComparingDataObserver::_End()
{
	if(this->IsSameData()) {
		mSameData = true;
		mCondition.notify_all();
	}
	mMutex.unlock();

}

void ComparingDataObserver::_Update(const Binary& arPoint, size_t aIndex)
{
	this->UpdateAny<Binary>(arPoint, aIndex, mpObserver->mBinaryMap, mBinaryMap);
}

void ComparingDataObserver::_Update(const Analog& arPoint, size_t aIndex)
{
	this->UpdateAny<Analog>(arPoint, aIndex, mpObserver->mAnalogMap, mAnalogMap);
}

void ComparingDataObserver::_Update(const Counter& arPoint, size_t aIndex)
{
	this->UpdateAny<Counter>(arPoint, aIndex, mpObserver->mCounterMap, mCounterMap);
}

void ComparingDataObserver::_Update(const ControlStatus& arPoint, size_t aIndex)
{
	this->UpdateAny<ControlStatus>(arPoint, aIndex, mpObserver->mControlStatusMap, mControlStatusMap);
}

void ComparingDataObserver::_Update(const SetpointStatus& arPoint, size_t aIndex)
{
	this->UpdateAny<SetpointStatus>(arPoint, aIndex, mpObserver->mSetpointStatusMap, mSetpointStatusMap);
}

}


