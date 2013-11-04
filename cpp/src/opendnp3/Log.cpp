
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
// you under the terms of the License.
//

#include <sstream>
#include <exception>

#include "Log.h"
#include "Thread.h"

#include <openpal/Exception.h>

using namespace std;
using namespace openpal;

namespace opendnp3
{



void EventLog::Log( const LogEntry& arEntry )
{
for(auto pair: mSubscribers) {
		if(this->SetContains(pair.second, -1) || this->SetContains(pair.second, arEntry.GetErrorCode())) {
			pair.first->Log(arEntry);
		}
	}
}

bool EventLog::SetContains(const std::set<int>& arSet, int aValue)
{
	return arSet.find(aValue) != arSet.end();
}


void EventLog :: AddLogSubscriber(ILogBase* apSubscriber)
{
	this->AddLogSubscriber(apSubscriber, -1);
}

void EventLog :: AddLogSubscriber(ILogBase* apSubscriber, int aErrorCode)
{
	SubscriberMap::iterator i = mSubscribers.find(apSubscriber);
	if(i == mSubscribers.end()) {
		std::set<int> set;
		mSubscribers.insert(SubscriberMap::value_type(apSubscriber, set));
		this->AddLogSubscriber(apSubscriber, aErrorCode);
	}
	else i->second.insert(aErrorCode);
}

void EventLog :: RemoveLogSubscriber(ILogBase* apBase)
{
	SubscriberMap::iterator i = mSubscribers.find(apBase);
	if(i != mSubscribers.end()) mSubscribers.erase(i);
}

}



