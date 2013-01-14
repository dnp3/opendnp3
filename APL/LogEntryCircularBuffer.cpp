
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
#include "LogEntryCircularBuffer.h"
#include <iostream>

namespace apl
{

LogEntryCircularBuffer :: LogEntryCircularBuffer(size_t aMaxEntries) :
	mMaxEntries(aMaxEntries)
{

}

size_t LogEntryCircularBuffer :: Count()
{
	std::unique_lock<std::mutex> lock(mMutex);
	return mItemQueue.size();
}

bool LogEntryCircularBuffer :: ReadLog(LogEntry& mEntry, int aTimeout)
{
	std::unique_lock<std::mutex> lock(mMutex);

	if(CheckRead(mEntry)) return true;
	else {
		if(aTimeout < 0) mCondition.wait(lock);
		else if(aTimeout > 0) mCondition.wait_for(lock, std::chrono::milliseconds(aTimeout));
		if(CheckRead(mEntry)) return true;
	}

	return false;
}

void LogEntryCircularBuffer :: BlockUntilEntry()
{
	std::unique_lock<std::mutex> lock(mMutex);
	if(mItemQueue.size() == 0) mCondition.wait(lock);
}

bool LogEntryCircularBuffer :: CheckRead(LogEntry& aEntry)
{
	if(mItemQueue.size() > 0) {
		aEntry = mItemQueue.front(); //make a copy of the front of the queue
		mItemQueue.pop_front();
		return true;
	}
	else return false;
}

void LogEntryCircularBuffer :: AddIgnoreCode(int aCode)
{
	mIgnoreCodes.insert(aCode);
}

void LogEntryCircularBuffer :: Log( const apl::LogEntry& arEntry )
{
	if(mIgnoreCodes.find(arEntry.GetErrorCode()) == mIgnoreCodes.end()) { //only log messages that aren't ignored

		size_t num = 0;
		{
			std::unique_lock<std::mutex> lock(mMutex);
			num = mItemQueue.size();
			mItemQueue.push_back(arEntry);
			if(mItemQueue.size() > mMaxEntries) {
				mItemQueue.pop_front();
			}
			mCondition.notify_one();
		}

		// only notify if the queue was empty
		if(num == 0) this->NotifyAll();
	}
}

void LogEntryCircularBuffer :: SetMaxEntries(size_t aMax)
{
	std::unique_lock<std::mutex> lock(mMutex);
	mMaxEntries = aMax;
	while(mItemQueue.size() > aMax) mItemQueue.pop_front();
}
}
