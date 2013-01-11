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


#ifndef _LOG_TO_FILE_H_
#define _LOG_TO_FILE_H_

#include <assert.h>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

#include "Log.h"
#include "LogEntryCircularBuffer.h"
#include "Thread.h"

namespace apl
{

/** Logging backend that uses a buffer and thread to do a non-blocking writeof all the log entries to a text file.
*/
class LogToFile: public LogEntryCircularBuffer, public Threadable
{
public:
	LogToFile(EventLog* apLog, const std::string aFileName, const bool aOverwriteFile = false);
	~LogToFile();
	void Run();

private:

	void SignalStop();
	void PushItemsToFile();
	void StartLogging();

	Thread* mpThread;
	EventLog* mpLog;
	std::string mFileName;
	bool mOverwriteFile;
};

}

#endif
