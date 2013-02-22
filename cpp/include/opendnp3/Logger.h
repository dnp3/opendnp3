
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
#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <string>
#include <vector>
#include <map>

#include "LogEntry.h"
#include "LogBase.h"


namespace opendnp3
{

class EventLog;

/**
* Interface that represents a distinct logger with a name and running level
*/
class Logger
{

public:
	Logger( EventLog* apLog, FilterLevel aFilter, const std::string& aName);

	void Log( FilterLevel aFilterLevel, const std::string& arLocation, const std::string& aMessage, int aErrorCode = -1);
	void Log( const LogEntry& arEntry);

	const std::string& GetName() const {
		return mName;
	}

	//functions for manipulating filter levels
	inline bool IsEnabled(FilterLevel aFilter) {
		return (mLevel & aFilter) != 0;
	}
	inline void SetFilters(int aLevel) {
		mLevel = aLevel;
	}
	void SetFilterLevel(FilterLevel aFilter);
	int GetFilters() {
		return mLevel;
	}

	Logger* GetSubLogger(std::string aName);
	Logger* GetSubLogger(std::string aSubName, int aFilterBits);
	Logger* GetSubLogger(std::string aSubName, FilterLevel aFilter);

private:

	int					mLevel;			// bit field describing what is being logged
	EventLog*			mpLog;
	std::string			mName;
};

/*
class LogCounter
{
public:
	LogCounter(Logger* apLogger, const std::string& arName) :
		mpLogger(apLogger),
		mName(arName),
		mValue(-1) {
		this->Increment();
	}

	void Increment() {
		mpLogger->Set(mName, ++mValue);
	}

private:
	Logger* mpLogger;
	std::string mName;
	int mValue;
};

class LogVariable
{
public:
	LogVariable(Logger* apLogger, const std::string& arName) :
		mpLogger(apLogger),
		mName(arName) {

	}

	void Set(int aVal) {
		mpLogger->Set(mName, aVal);
	}

private:
	Logger* mpLogger;
	std::string mName;
};
*/

}

#endif

