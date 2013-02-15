
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
#ifndef __LOG_ENTRY_H_
#define __LOG_ENTRY_H_

#include "LogTypes.h"
#include "Parsing.h"
#include "Util.h"
#include "Types.h"

#include <chrono>
#include <map>

namespace opendnp3
{

/**
* An event recorded by the logging framework
*/
class LogEntry
{
	typedef std::map<std::string, std::string> KeyValueMap;

public:

	LogEntry(): mTime(std::chrono::high_resolution_clock::now()) {};

	LogEntry( FilterLevel aLevel, const std::string& aDeviceName, const std::string& aLocation, const std::string& aMessage, int aErrorCode);

	/// @return The name of the logger that recorded the message
	const std::string&	GetDeviceName() const {
		return mDeviceName;
	}

	/// @return The place in the source code where the message was recorded
	const std::string&	GetLocation() const {
		return mLocation;
	}

	/// @return body of the log message
	const std::string&	GetMessage() const {
		return mMessage;
	}

	/// @return the log level of the message
	FilterLevel			GetFilterLevel() const {
		return mFilterLevel;
	}

	/// @return the timestamp of the message as a string
	std::string			GetTimeString() const {
		return ToNormalizedString(mTime);
	}

	/// @return the timestamp of the message 
	std::chrono::high_resolution_clock::time_point GetTimeStamp() const {
		return mTime;
	}

	/// @return the error code associated with the message
	int					GetErrorCode() const {
		return mErrorCode;
	}

	/// @param aUseLocation if true, the location information is incorporated
	/// @return String representation of the entire entry
	std::string			LogString(bool aUseLocation = false) const;

	/**
	* Retrieve a value of the attribute map
	* @param arKey key of the value to retrieve
	* @param arValue reference to be written if the value is present
	* @return true if present, false otherwise
	*/
	bool GetValue(const std::string& arKey, std::string& arValue) const;
	
	/**
	* Retrieve a value of the attribute map
	* @param arKey key of the value to retrieve
	* @param arValue reference to be written if the value is present
	* @return true if present, false otherwise
	*/
	bool GetValue(const std::string& arKey, int& arValue) const;
	
	/**
	* Retrieve a value of the attribute map
	* @param arKey key of the value to retrieve
	* @param arValue reference to be written if the value is present
	* @return true if present, false otherwise
	*/
	bool GetValue(const std::string& arKey, int64_t& arValue) const;

	/**
	* Add a key value pair to the attribute map
	* @param arKey key of the value to write
	* @param arValue value to be written
	*/
	void AddValue(const std::string& arKey, const std::string& arValue);
	
	/**
	* Add a key value pair to the attribute map
	* @param arKey key of the value to write
	* @param arValue value to be written
	*/
	void AddValue(const std::string& arKey, int aValue);
	
	/**
	* Add a key value pair to the attribute map
	* @param arKey key of the value to write
	* @param arValue value to be written
	*/
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
