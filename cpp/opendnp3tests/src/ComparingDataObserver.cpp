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


