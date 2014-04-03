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
#include "LogToStdio.h"

#include <chrono>
#include <sstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace std::chrono;

namespace asiopal
{

LogToStdio LogToStdio::instance;


LogToStdio::LogToStdio() : pInterpreter(&BasicFlags), printLocation(false)
{
}

std::ostringstream& LogToStdio::BasicFlags(std::ostringstream& ss, const openpal::LogFilters& filters)
{
	ss << filters.GetBitfield();
	return ss;
}

void LogToStdio::SetPrintLocation(bool printLocation_)
{
	printLocation = printLocation_;
}

void LogToStdio::SetLevelInterpreter(LevelToString pInterpreter_)
{
	assert(pInterpreter_ != nullptr);
	pInterpreter = pInterpreter_;
}

void LogToStdio::Log(const openpal::LogEntry& entry)
{
	auto time = std::chrono::high_resolution_clock::now();
	auto num = duration_cast<milliseconds>(time.time_since_epoch()).count();

	ostringstream oss;

	oss << num << " - ";
	pInterpreter(oss, entry.GetFilters());
	oss << " - " << entry.GetName();
	if (printLocation) oss << " - " << entry.GetLocation();
	oss << " - " << entry.GetMessage();

	if(entry.GetErrorCode() != -1) oss << " - " << entry.GetErrorCode();

	std::unique_lock<std::mutex> lock(mutex);
	std::cout << oss.str() << std::endl;
}

} //end ns

