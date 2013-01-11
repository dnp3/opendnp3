
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
#include "ChangeBuffer.h"

#include <assert.h>

namespace apl 
{

size_t ChangeBuffer::FlushUpdates(apl::IDataObserver* apObserver, bool aClear)
{
	assert(this->InProgress());
	size_t count = 0;
	if(!this->HasChanges()) return count;

	{
		Transaction t(apObserver);
		mMidFlush = true;	// Will clear on transaction end if an observer call blows up
		for(auto f: mChangeQueue) 
		{
			++count;
			f(apObserver);			
		}

		mMidFlush = false;
	}

	if(aClear) this->Clear();

	return count;
}

void ChangeBuffer::_Start() 
{
	mMutex.lock();
}

void ChangeBuffer::_End() {

	if ( mMidFlush ) {
		_Clear();
		mMidFlush = false;
	}

	bool notify = this->HasChanges();
	mMutex.unlock();
	if(notify) this->NotifyAll();
}

void ChangeBuffer::_Update(const Binary& arPoint, size_t aIndex) 
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Binary>, std::placeholders::_1, arPoint, aIndex));
}

void ChangeBuffer::_Update(const Analog& arPoint, size_t aIndex) {
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Analog>, std::placeholders::_1, arPoint, aIndex));
}

void ChangeBuffer::_Update(const Counter& arPoint, size_t aIndex) {		
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Counter>, std::placeholders::_1, arPoint, aIndex));		
}

void ChangeBuffer::_Update(const ControlStatus& arPoint, size_t aIndex) {				
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<ControlStatus>, std::placeholders::_1, arPoint, aIndex));
}

void ChangeBuffer::_Update(const SetpointStatus& arPoint, size_t aIndex) {
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<SetpointStatus>, std::placeholders::_1, arPoint, aIndex));		
}

void ChangeBuffer::Clear() {
	assert(this->InProgress());
	_Clear();
}

void ChangeBuffer::_Clear() {
	mChangeQueue.clear();
}

bool ChangeBuffer::HasChanges() {
	return mChangeQueue.size() > 0;
}

}


