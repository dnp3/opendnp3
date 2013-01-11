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
#include "CommandManager.h"

#include "Exception.h"
#include "Location.h"

#include <assert.h>

using namespace std;

namespace apl
{
void CommandManager::DefineCommand(const string& arName, CommandTypes aType, size_t aIndex, ICommandAcceptor* apCmdAcceptor)
{
	assert(apCmdAcceptor != NULL);

	CommandInfo info;
	info.mType = aType;
	info.mpCmdAcceptor = apCmdAcceptor;
	info.mIndex = aIndex;
	pair<string, CommandInfo> entry(arName, info);

	if(!this->mCmdMap.insert(entry).second) {
		throw Exception(LOCATION, "Command already defined with name: " + arName);
	}
}

CommandInfo CommandManager::GetMapping(const std::string& arName)
{
	CmdMap::iterator i = mCmdMap.find(arName);
	if(i == mCmdMap.end()) throw Exception(LOCATION, "Command not previously mapped");
	return i->second;
}

}

