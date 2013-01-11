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
#ifndef __SUBJECT_BASE_H_
#define __SUBJECT_BASE_H_


#include "ISubject.h"
#include "INotifier.h"
#include <set>
#include <mutex>

namespace apl
{

template <class LockType>
class SubjectBase : public ISubject
{
	typedef std::set<INotifier*> NotifierSet;

public:
	virtual ~SubjectBase() {}

	// implement the ISubject interface
	void AddObserver(INotifier* apNotifier) {
		std::lock_guard<LockType> lock(mSubjectMutex);
		mNotifiers.insert(apNotifier);
	}

	void RemoveObserver(INotifier* apNotifier) {
		std::lock_guard<LockType> lock(mSubjectMutex);
		mNotifiers.erase(apNotifier);
	}

protected:

	void NotifyAll() {
		std::lock_guard<LockType> lock(mSubjectMutex);		
		for(auto pNotifier : mNotifiers) pNotifier->Notify();		
	}

private:
	LockType mSubjectMutex;
	NotifierSet mNotifiers;
};

}

#endif
