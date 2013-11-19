
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
// you under the terms of the License.
//

#include "ConstantCommandProcessor.h"

#include <openpal/IExecutor.h>

using namespace openpal;

namespace opendnp3
{

ConstantCommandProcessor::ConstantCommandProcessor(IExecutor* apExecutor, const CommandResponse& arResponse) :
	mpExecutor(apExecutor),
	mResponse(arResponse)
{

}


void ConstantCommandProcessor::SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

void ConstantCommandProcessor::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	CommandResponse cr(mResponse);
	mpExecutor->Post([ = ]() {
		aCallback(cr);
	});
}

}

