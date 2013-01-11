
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
#ifndef __LOG_TERMINAL_EXTENSION_H_
#define __LOG_TERMINAL_EXTENSION_H_

#include <APL/Log.h>
#include <APL/LogToFile.h>
#include <APL/MetricBuffer.h>

#include "TerminalInterfaces.h"

namespace apl
{

class IExecutor;
class ITimer;

class LogTerminalExtension : public ITerminalExtension, public INotifier
{
public:
	LogTerminalExtension(EventLog*);

	virtual ~LogTerminalExtension();

	void RedirectedLine(const std::string&);

	// Implements INotifier so that we can process new log messages
	void Notify();

private:

	EventLog* mpLog;
	IExecutor* mpExecutor;

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);

	std::string GetLogString(LogEntry& arEntry);
	void ResetActiveColumns();
	void HandleLogTimer();

	enum LogColumns {
	    COL_TIME,
	    COL_FILTER,
	    COL_DEVICE,
	    COL_LOCATION,
	    COL_MESSAGE
	};

	void StopLoggingToFile();

	std::vector<LogColumns> mActiveColumns;
	LogEntryCircularBuffer mBuffer;
	MetricBuffer mMetrics;
	LogToFile* mpFileLogger;

	//print
	retcode HandlePrintLog(std::vector<std::string>&);
	retcode HandlePrintLoggers(std::vector<std::string>&);
	retcode HandlePrintVars(std::vector<std::string>&);
	//run
	retcode HandleRunLog(std::vector<std::string>& arTokens);
	//set
	retcode HandleSetFilterOrLevel(std::vector<std::string>& arTokens, bool aSetLevel);
	retcode HandleSetLogCol(std::vector<std::string>& arTokens);

	retcode HandleStartToFileLogging(std::vector<std::string>& arTokens);
	retcode HandleStopToFileLogging(std::vector<std::string>& arTokens);

	void PrintLog();
};
}

#endif

