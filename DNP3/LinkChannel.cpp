
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
#include "LinkChannel.h"

#include "Stack.h"

#include <APL/Logger.h>
#include <APL/LoggableMacros.h>
#include <APL/IPhysicalLayerAsync.h>
#include <APL/AsyncTaskGroup.h>
#include <APL/GetKeys.h>

namespace apl
{
namespace dnp
{

LinkChannel::LinkChannel(Logger* apLogger, const std::string& arName, IPhysicalLayerAsync* apPhys, AsyncTaskGroup* apTaskGroup, millis_t aOpenRetry) :
	Loggable(apLogger),
	LinkLayerRouter(apLogger, apPhys, aOpenRetry),
	mName(arName),
	mpTaskGroup(apTaskGroup)
{

}

void LinkChannel::BindStackToChannel(const std::string& arStackName, Stack* apStack, const LinkRoute& arRoute)
{
	LOG_BLOCK(LEV_DEBUG, "Linking stack to port w/ route " << arRoute);
	apStack->mLink.SetRouter(this);
	this->AddContext(&apStack->mLink, arRoute); // this function can throw, do it before adjusting the map
	mStackMap[arStackName] = StackRecord(apStack, arRoute);
}

void LinkChannel::RemoveStackFromChannel(const std::string& arStackName)
{
	StackMap::iterator i = mStackMap.find(arStackName);
	if(i == mStackMap.end()) throw ArgumentException("Stack with that name not bound: " + arStackName);
	else {
		StackRecord r = i->second;
		LOG_BLOCK(LEV_DEBUG, "Unlinking stack from port w/ route " << r.route);
		this->RemoveContext(r.route);
		mStackMap.erase(i);
	}
}

std::vector<std::string> LinkChannel::StacksOnChannel()
{
	return GetKeys<StackMap, std::string>(mStackMap);
}

}
}
