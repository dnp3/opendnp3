
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

#include "QueuedCommandProcessor.h"

namespace opendnp3
{

QueuedCommandProcessor::QueuedCommandProcessor() :
	SubjectBase()
{

}

void QueuedCommandProcessor::SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)	
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->SelectAndOperateT(arCommand, aIndex, aCallback);
}

void QueuedCommandProcessor::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->DirectOperateT(arCommand, aIndex, aCallback);
}

bool QueuedCommandProcessor::Dispatch(ICommandProcessor* apProcessor)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mRequestQueue.empty()) return false;
	else {
		mRequestQueue.front()(apProcessor);
		mRequestQueue.pop();
		return true;
	}
}

}

