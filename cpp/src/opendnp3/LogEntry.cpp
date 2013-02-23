
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
#include <opendnp3/LogEntry.h>

#include <sstream>
#include <opendnp3/Util.h>
#include <opendnp3/Parsing.h>

using namespace std;

namespace opendnp3
{


LogEntry::LogEntry( FilterLevel aLevel, const std::string& aDeviceName, const std::string& aLocation, const std::string& aMessage, int aErrorCode)
	:
	mFilterLevel(aLevel),
	mDeviceName(aDeviceName),
	mLocation(aLocation),
	mMessage(aMessage),
	mTime(std::chrono::steady_clock::now()),
	mErrorCode(aErrorCode)
{
}

void LogEntry :: AddKeyValue(const std::string& arKey, const std::string& arValue)
{
	mKeyValues.insert(KeyValueMap::value_type(arKey, arValue));
}

void LogEntry :: AddValue(const std::string& arKey, int aValue)
{
	this->AddAnyValue(arKey, aValue);
}

void LogEntry :: AddValue(const std::string& arKey, int64_t aValue)
{
	this->AddAnyValue(arKey, aValue);
}

void LogEntry :: AddValue(const std::string& arKey, const std::string& arValue)
{
	this->AddKeyValue(arKey, arValue);
}

bool LogEntry :: GetValue(const std::string& arKey, std::string& arValue) const
{
	KeyValueMap::const_iterator i = mKeyValues.find(arKey);
	if(i == mKeyValues.end()) return false;
	else {
		arValue = i->second;
		return true;
	}
}

bool LogEntry :: GetValue(const std::string& arKey, int& arValue) const
{
	return GetAnyValue<int>(arKey, arValue);
}

bool LogEntry :: GetValue(const std::string& arKey, int64_t& arValue) const
{
	return GetAnyValue<int64_t>(arKey, arValue);
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
string LogEntry :: LogString(bool aUseLocation) const
{
	ostringstream oss;
	oss << GetTimeString() << " - "
	    << LogTypes::GetLevelString( mFilterLevel ) << " - "
	    << mDeviceName;
	if(aUseLocation && !mLocation.empty()) oss << " - " << mLocation;
	oss << " - " << mMessage;

	if(this->GetErrorCode() != -1) oss << " - " << this->GetErrorCode();

	return oss.str();
}
#endif

}

