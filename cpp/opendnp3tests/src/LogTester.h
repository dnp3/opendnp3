/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __LOG_TESTER_H_
#define __LOG_TESTER_H_

#include <openpal/LogRoot.h>
#include <queue>

namespace opendnp3
{

class LogTester : public openpal::ILogBase
{

public:
	LogTester();

	void Log(const std::string& aLocation, const std::string& aMessage);

	void Log( const openpal::LogEntry& arEntry );

	bool PopOneEntry(int32_t filter);

	bool PopUntil(int32_t filter);

	int ClearLog();
	int NextErrorCode();
	bool GetNextEntry(openpal::LogEntry& arEntry);
	bool IsLogErrorFree();

	void Pop(openpal::ILogBase* pLog);

	openpal::Logger GetLogger(const std::string& id);

protected:

	openpal::LogRoot root;
	openpal::Logger logger;
	std::queue<openpal::LogEntry> mBuffer;

};


}

#endif
