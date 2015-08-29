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
#ifndef ASIOPAL_LOGFANOUTHANDLER_H
#define ASIOPAL_LOGFANOUTHANDLER_H

#include <mutex>
#include <vector>

#include <openpal/logging/ILogHandler.h>
#include <openpal/util/Uncopyable.h>

namespace asiopal
{

/**
	A simple proxy that fans out log messages to multiple subscribers
*/
class LogFanoutHandler : public openpal::ILogHandler, private openpal::Uncopyable
{
public:	

	/**
	* Binds a listener to ALL log messages
	*/
	void Subscribe(openpal::ILogHandler* pHandler);	

	// implement the log function from ILogHandler
	virtual void Log(const openpal::LogEntry& entry) override final;

private:	

	std::mutex callbackMutex;
	std::vector<openpal::ILogHandler*> subscribers;
};


}

#endif

