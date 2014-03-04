/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
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
#include <opendnp3/outstation/ICommandHandler.h>

class CommandHandlerAdapter : public opendnp3::ICommandHandler
{
public:
	CommandHandlerAdapter(JavaVM* apJVM, jobject aProxy);

	opendnp3::CommandStatus Supports(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex);
	opendnp3::CommandStatus Perform(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex);
	

	opendnp3::CommandStatus Supports(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex);
	opendnp3::CommandStatus Perform(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex);
	

	opendnp3::CommandStatus Supports(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex);
	opendnp3::CommandStatus Perform(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex);
	

	opendnp3::CommandStatus Supports(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex);
	opendnp3::CommandStatus Perform(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex);
	

	opendnp3::CommandStatus Supports(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex);
	opendnp3::CommandStatus Perform(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex);
	

private:

	jmethodID mSupportsCROB;
	jmethodID mPerformCROB;
	

	jmethodID mSupportsAnalogInt16;
	jmethodID mPerformAnalogInt16;
	

	jmethodID mSupportsAnalogInt32;
	jmethodID mPerformAnalogInt32;
	

	jmethodID mSupportsAnalogFloat32;
	jmethodID mPerformAnalogFloat32;
	

	jmethodID mSupportsAnalogDouble64;
	jmethodID mPerformAnalogDouble64;
	

	JNIEnv* GetEnv();

	JavaVM* mpJVM;
	jobject mProxy;
};

#endif
