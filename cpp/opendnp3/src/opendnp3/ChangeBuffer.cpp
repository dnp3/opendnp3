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
#include "ChangeBuffer.h"

#include <assert.h>

namespace opendnp3
{

size_t ChangeBuffer::FlushUpdates(IDataObserver* apObserver)
{
	size_t count = Transaction::Apply<size_t>(*this, 
		[apObserver](const ChangeBuffer& cb) {
			size_t cnt = cb.mChangeQueue.size();
			if(cnt > 0) {
				Transaction t(apObserver);
				for(auto& f: cb.mChangeQueue) f(apObserver);
			}
			return cnt;
		}
	);	
	this->Clear();
	return count;
}

void ChangeBuffer::Start()
{
	mMutex.lock();
}

void ChangeBuffer::End()
{
	bool notify = mNotify;
	mNotify = false;
	mMutex.unlock();
	if(notify) this->NotifyObservers();
}

void ChangeBuffer::Update(const Binary& arPoint, uint16_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Binary>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::Update(const Analog& arPoint, uint16_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Analog>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::Update(const Counter& arPoint, uint16_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Counter>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::Update(const FrozenCounter& arPoint, uint16_t aIndex)
{
    mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<FrozenCounter>, std::placeholders::_1, arPoint, aIndex));
    mNotify = true;
}

void ChangeBuffer::Update(const BinaryOutputStatus& arPoint, uint16_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<BinaryOutputStatus>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::Update(const AnalogOutputStatus& arPoint, uint16_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<AnalogOutputStatus>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::Clear()
{
	mChangeQueue.clear();
}

}


