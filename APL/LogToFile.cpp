
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
#include "LogToFile.h"
#include "Util.h"
#include "Location.h"

namespace apl
{



LogToFile :: LogToFile(const std::string aFileName, const bool aOverwriteFile)
	: LogEntryCircularBuffer(1000), mpThread(NULL), mFileName(aFileName), mOverwriteFile(aOverwriteFile)
{
	if(aFileName == "-" || aFileName == "") {
		mpLog = NULL;
	}
	else {
		StartLogging();
	}
}

void LogToFile :: StartLogging()
{
	LogEntry le(LEV_EVENT, "FileLogger", LOCATION, "New Log Started", -1);
	Log(le);

	mpThread = new Thread(this);
	mpThread->Start();	
}

LogToFile :: ~LogToFile()
{
	if(mpThread != NULL) {
		mpThread->RequestStop();
		mpThread->WaitForStop();
	}

	delete mpThread;	
}

void LogToFile :: SignalStop()
{
	std::unique_lock<std::mutex> lock(mMutex);
	mCondition.notify_one();
}

/**
open, write, close the file rather than hold it forever so it can handle having
the file deleted from underneath it without failing.
*/
void LogToFile :: PushItemsToFile()
{
	try {
		// we need to open file in append mode first time only
		std::ofstream file(mFileName.c_str(), mOverwriteFile ? (std::ios::out) : (std::ios::app | std::ios::out));
		if(!file.is_open()) std::cerr << "Failure to open: " << mFileName << std::endl;

		mOverwriteFile = false;

		LogEntry le;
		while(ReadLog(le)) file << le.LogString() << std::endl;

		file << std::flush;
		if(file.bad()) std::cerr << "Failure during writing log file: " << file.rdstate() << std::endl;
		file.close();
	}
	catch(std::exception e) {
		std::cerr << "Error during LogToFile: " << e.what() << std::endl;
	}
}

void LogToFile :: Run()
{
	while(!this->IsExitRequested()) {
		this->BlockUntilEntry();
		this->PushItemsToFile();
	}
}
}
