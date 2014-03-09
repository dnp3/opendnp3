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
#include "CommandHandlerAdapter.hpp"
#include "JNIHelpers.hpp"

#include <assert.h>
#include <iostream>

using namespace opendnp3;

CommandHandlerAdapter::CommandHandlerAdapter(JavaVM* apJVM, jobject aProxy) : mpJVM(apJVM), mProxy(aProxy)
{
	auto pEnv = GetEnv();
	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != nullptr);

	mSupportsCROB = JNIHelpers::GetMethodID(pEnv, clazz, "Supports",  "(ISJJIJ)I");
	mPerformCROB = JNIHelpers::GetMethodID(pEnv, clazz, "Perform",  "(ISJJIJ)I");


	mSupportsAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "Supports", "(SJ)I");
	mPerformAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "Perform", "(SJ)I");


	mSupportsAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "Supports", "(IJ)I");
	mPerformAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "Perform", "(IJ)I");


	mSupportsAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "Supports", "(FJ)I");
	mPerformAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "Perform", "(FJ)I");


	mSupportsAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "Supports", "(DJ)I");
	mPerformAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "Perform", "(DJ)I");


}

CommandStatus CommandHandlerAdapter::Supports(const ControlRelayOutputBlock& arCommand, uint16_t aIndex)
{
	jint func = ControlCodeToType(arCommand.functionCode);
	jshort count = arCommand.count;
	jlong ontime = arCommand.onTimeMS;
	jlong offtime = arCommand.offTimeMS;
	jint status = CommandStatusToType(arCommand.status);
	jlong index = aIndex;

	jint ret = this->GetEnv()->CallIntMethod(mProxy, mSupportsCROB, func, count, ontime, offtime, status, index);
	return CommandStatusFromType(ret);
}

CommandStatus CommandHandlerAdapter::Perform(const ControlRelayOutputBlock& arCommand, uint16_t aIndex)
{
	jint func = ControlCodeToType(arCommand.functionCode);
	jshort count = arCommand.count;
	jlong ontime = arCommand.onTimeMS;
	jlong offtime = arCommand.offTimeMS;
	jint status = CommandStatusToType(arCommand.status);
	jlong index = aIndex;

	jint ret = this->GetEnv()->CallIntMethod(mProxy, mPerformCROB, func, count, ontime, offtime, status, index);
	return CommandStatusFromType(ret);
}

CommandStatus CommandHandlerAdapter::Supports(const AnalogOutputInt16& arCommand, uint16_t aIndex)
{
	jshort value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSupportsAnalogInt16, value, index);
	return CommandStatusFromType(status);
}

CommandStatus CommandHandlerAdapter::Perform(const AnalogOutputInt16& arCommand, uint16_t aIndex)
{
	jshort value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mPerformAnalogInt16, value, index);
	return CommandStatusFromType(status);
}

CommandStatus CommandHandlerAdapter::Supports(const AnalogOutputInt32& arCommand, uint16_t aIndex)
{
	jshort value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSupportsAnalogInt32, value, index);
	return CommandStatusFromType(status);
}
CommandStatus CommandHandlerAdapter::Perform(const AnalogOutputInt32& arCommand, uint16_t aIndex)
{
	jint value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mPerformAnalogInt32, value, index);
	return CommandStatusFromType(status);
}

CommandStatus CommandHandlerAdapter::Supports(const AnalogOutputFloat32& arCommand, uint16_t aIndex)
{
	jfloat value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSupportsAnalogFloat32, value, index);
	return CommandStatusFromType(status);
}

CommandStatus CommandHandlerAdapter::Perform(const AnalogOutputFloat32& arCommand, uint16_t aIndex)
{
	jfloat value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mPerformAnalogFloat32, value, index);
	return CommandStatusFromType(status);
}

CommandStatus CommandHandlerAdapter::Supports(const AnalogOutputDouble64& arCommand, uint16_t aIndex)
{
	jdouble value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSupportsAnalogDouble64, value, index);
	return CommandStatusFromType(status);
}

CommandStatus CommandHandlerAdapter::Perform(const AnalogOutputDouble64& arCommand, uint16_t aIndex)
{
	jdouble value = arCommand.value;
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mPerformAnalogDouble64, value, index);
	return CommandStatusFromType(status);
}

JNIEnv* CommandHandlerAdapter::GetEnv()
{
	return JNIHelpers::GetEnvFromJVM(mpJVM);
}





