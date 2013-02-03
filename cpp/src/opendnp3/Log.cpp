
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
#include <iostream>
#include <sstream>
#include <exception>

#include "Log.h"
#include "Thread.h"
#include "Exception.h"

using namespace std;

namespace opendnp3
{

EventLog::~EventLog()
{
	for(auto pair: mLogMap) delete pair.second;
}

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

Logger* EventLog::GetLogger(FilterLevel aFilter, const std::string& arName)
{
	Logger* pLogger = GetExistingLogger(arName);

	if(pLogger != NULL) return pLogger;

	{
		std::unique_lock<std::mutex> lock(mMutex);
		assert(mLogMap.find(arName) == mLogMap.end());

		pLogger = new Logger(this, aFilter, arName);
		mLogMap[arName] = pLogger;
	}

	return pLogger;
}

Logger* EventLog::GetExistingLogger( const std::string& arLoggerName )
{
	std::unique_lock<std::mutex> lock(mMutex);
	Logger* pLogger = NULL;
	LoggerMap::iterator i = mLogMap.find(arLoggerName);
	if(i != mLogMap.end()) pLogger = i->second;
	return pLogger;
}

void EventLog::GetAllLoggers( std::vector<Logger*>& apLoggers)
{
	apLoggers.clear();
	std::unique_lock<std::mutex> lock(mMutex);
	for(auto pair: mLogMap) {	
		apLoggers.push_back(pair.second);
	}
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



