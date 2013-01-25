
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
#include "LogAdapter.h"

#include "Conversions.h"

namespace DNP3
{	
namespace Adapter
{		
	

LogAdapter::LogAdapter(ILogHandler^ proxy) : proxy(proxy) 
{}

// logging error messages, etc
void LogAdapter::Log( const apl::LogEntry& arEntry )
{
	FilterLevel level = Conversions::convertFilterLevel(arEntry.GetFilterLevel());
	System::String^ loggerName = Conversions::convertString(arEntry.GetDeviceName());
	System::String^ location = Conversions::convertString(arEntry.GetLocation());
	System::String^ message = Conversions::convertString(arEntry.GetMessage());
	System::DateTime time = TimeStamp::Convert(std::chrono::duration_cast<std::chrono::milliseconds>(arEntry.GetTimeStamp().time_since_epoch()).count());
	LogEntry^ le = gcnew LogEntry(level, loggerName, location, message, time, arEntry.GetErrorCode());
	
	proxy->Log(le);
}
		
}}

