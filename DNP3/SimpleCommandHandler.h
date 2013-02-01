
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
#ifndef __SIMPLE_COMMAND_HANDLER_H_
#define __SIMPLE_COMMAND_HANDLER_H_

#include "ICommandHandler.h"

#include <functional>

namespace apl
{
namespace dnp
{


class SimpleCommandHandler : public ICommandHandler
{
	public:

		SimpleCommandHandler(std::function<CommandStatus ()> aStatusFunc);

		CommandStatus Select(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex);

		CommandStatus Select(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus Operate(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex);

		CommandStatus Select(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus Operate(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex);
	
		CommandStatus Select(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus Operate(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex);

		CommandStatus Select(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus Operate(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence);
		CommandStatus DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex);

	private:
		std::function<CommandStatus ()> mStatusFunc;
};

class SuccessCommandHandler : public SimpleCommandHandler
{
	public:
		static SuccessCommandHandler* Inst() { return &mHandler; }

	private:
		static SuccessCommandHandler mHandler;

	protected:
		SuccessCommandHandler();
};

}
}

#endif
