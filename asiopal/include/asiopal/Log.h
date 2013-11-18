
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

#ifndef __EVENT_LOG_H_
#define __EVENT_LOG_H_


#include <assert.h>
#include <map>
#include <vector>
#include <mutex>
#include <set>

#include <openpal/LogBase.h>
#include <openpal/Visibility.h>

namespace asiopal
{

class DLL_LOCAL EventLog : public openpal::ILogBase
{
public:	

	/**
	* Binds a listener to ALL log messages
	*/
	void AddLogSubscriber(ILogBase* apSubscriber);

	/**
	* Binds a listener to only certain error messages
	*/
	void AddLogSubscriber(ILogBase* apSubscriber, int aErrorCode);

	/**
	* Cancels a previous binding
	*/
	void RemoveLogSubscriber(ILogBase* apBase);

	//implement the log function from ILogBase
	void Log( const openpal::LogEntry& arEntry );

private:

	bool SetContains(const std::set<int>& arSet, int aValue);

	std::mutex mMutex;	
	typedef std::map<ILogBase*, std::set<int> > SubscriberMap;
	SubscriberMap mSubscribers;

};


}

#endif

