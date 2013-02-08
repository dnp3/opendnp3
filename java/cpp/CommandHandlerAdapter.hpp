/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#ifndef __COMMAND_HANDLER_ADAPTER_H_
#define __COMMAND_HANDLER_ADAPTER_H_

#include <jni.h>
#include <opendnp3/ICommandHandler.h>

class CommandHandlerAdapter : public opendnp3::ICommandHandler
{
	public:
	CommandHandlerAdapter(JavaVM* apJVM, jobject aProxy);

	opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus DirectOperate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex);

	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex);

	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex);

	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex);

	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex);
	
	private:

	jmethodID mSelectCROB;
	jmethodID mOperateCROB;
	jmethodID mDirectOperateCROB;

	jmethodID mSelectAnalogInt16;
	jmethodID mOperateAnalogInt16;
	jmethodID mDirectOperateAnalogInt16;

	jmethodID mSelectAnalogInt32;
	jmethodID mOperateAnalogInt32;
	jmethodID mDirectOperateAnalogInt32;

	jmethodID mSelectAnalogFloat32;
	jmethodID mOperateAnalogFloat32;
	jmethodID mDirectOperateAnalogFloat32;

	jmethodID mSelectAnalogDouble64;
	jmethodID mOperateAnalogDouble64;
	jmethodID mDirectOperateAnalogDouble64;

	JNIEnv* GetEnv();

	JavaVM* mpJVM;
	jobject mProxy;	
};

#endif
