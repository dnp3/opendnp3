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
#include "Logger.h"

#include "LogBase.h"
#include "LogRoot.h"

#include <assert.h>
#include <cstring>

using namespace std;

namespace openpal
{

Logger::Logger(LogRoot* pRoot_, int subType_) :
pRoot(pRoot_),
subType(subType_)
{
	
}

bool Logger::IsEnabled(const LogFilters& filters) const
{
	return pRoot->IsEnabled(filters);
}

Logger Logger::SwitchType(int subType) const
{
	return Logger(pRoot, subType);
}

void Logger::Log(const LogFilters& filters, bool first, char const* location, char const* message, int errorCode)
{
	if (pRoot->IsEnabled(filters))
	{		
		pRoot->Log(filters, subType, first, location, message, errorCode);
	}
}

}

