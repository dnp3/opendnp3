//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __FANOUT_DATA_OBSERVER_H_
#define __FANOUT_DATA_OBSERVER_H_

#include <APL/ChangeBuffer.h>

namespace apl
{

class FanoutDataObserver : public IDataObserver
{
public:

	void AddObserver(IDataObserver* apObserver) {
		mObservers.push_back(apObserver);
	}

	void _Start() {
		mBuffer.Start();
	}
	void _End() {
		for(IDataObserver * p: mObservers) {
			mBuffer.FlushUpdates(p, false);
		}
		mBuffer.Clear();
		mBuffer.End();				
	}

	void _Update(const Binary& arPoint, size_t aIndex) {
		mBuffer.Update(arPoint, aIndex);
	}
	void _Update(const Analog& arPoint, size_t aIndex) {
		mBuffer.Update(arPoint, aIndex);
	}
	void _Update(const Counter& arPoint, size_t aIndex) {
		mBuffer.Update(arPoint, aIndex);
	}
	void _Update(const ControlStatus& arPoint, size_t aIndex) {
		mBuffer.Update(arPoint, aIndex);
	}
	void _Update(const SetpointStatus& arPoint, size_t aIndex) {
		mBuffer.Update(arPoint, aIndex);
	}

private:
	ChangeBuffer mBuffer;
	std::vector<IDataObserver*> mObservers;
};

}

#endif

