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
#ifndef __LOG_ENTRY_H_
#define __LOG_ENTRY_H_

#include "LogTypes.h"
#include "Parsing.h"
#include "Types.h"
#include "Util.h"
#include <chrono>

#include <map>

namespace apl
{

class LogEntry
{
	typedef std::map<std::string, std::string> KeyValueMap;

public:

	LogEntry(): mTime(std::chrono::high_resolution_clock::now()) {};

	LogEntry( FilterLevel aLevel, const std::string& aDeviceName, const std::string& aLocation, const std::string& aMessage, int aErrorCode);

	const std::string&	GetDeviceName() const {
		return mDeviceName;
	}
	const std::string&	GetLocation() const {
		return mLocation;
	}
	const std::string&	GetMessage() const {
		return mMessage;
	}
	FilterLevel			GetFilterLevel() const {
		return mFilterLevel;
	}
	std::string			GetTimeString() const {
		return ToNormalizedString(mTime);
	}

	std::chrono::high_resolution_clock::time_point GetTimeStamp() const {
		return mTime;
	}

	int					GetErrorCode() const {
		return mErrorCode;
	}
	std::string			LogString(bool aUseLocation = false) const;

	bool GetValue(const std::string& arKey, std::string& arValue) const;
	bool GetValue(const std::string& arKey, int& arValue) const;
	bool GetValue(const std::string& arKey, int64_t& arValue) const;

	void AddValue(const std::string& arKey, const std::string& arValue);
	void AddValue(const std::string& arKey, int aValue);
	void AddValue(const std::string& arKey, int64_t aValue);

private:

	void AddKeyValue(const std::string& arKey, const std::string& arValue);

	template <class T>
	void AddAnyValue(const std::string& arKey, const T& arValue) {
		std::ostringstream oss;
		oss << arValue;
		AddKeyValue(arKey, oss.str());
	}

	template <class T>
	bool GetAnyValue(const std::string& arKey, T& arValue) const {
		std::string text;
		if(GetValue(arKey, text)) {
			Parsing::Get(text, arValue);
			return true;
		}
		else return false;
	}

	FilterLevel		mFilterLevel;
	std::string		mDeviceName;
	std::string		mLocation;
	std::string		mMessage;
	std::chrono::high_resolution_clock::time_point mTime;
	int				mErrorCode;
	KeyValueMap		mKeyValues;
};

}

#endif
