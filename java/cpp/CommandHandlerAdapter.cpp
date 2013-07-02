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
	assert(clazz != NULL);

	mSelectCROB = JNIHelpers::GetMethodID(pEnv, clazz, "Select",  "(ISJJIJ)I");
	mOperateCROB = JNIHelpers::GetMethodID(pEnv, clazz, "Operate",  "(ISJJIJ)I");
	mDirectOperateCROB = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate",  "(ISJJIJ)I");

	mSelectAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(SJ)I");
	mOperateAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(SJ)I");
	mDirectOperateAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(SJ)I");

	mSelectAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(IJ)I");
	mOperateAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(IJ)I");
	mDirectOperateAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(IJ)I");

	mSelectAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(FJ)I");
	mOperateAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(FJ)I");
	mDirectOperateAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(FJ)I");

	mSelectAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(DJ)I");
	mOperateAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(DJ)I");
	mDirectOperateAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(DJ)I");

}

CommandStatus CommandHandlerAdapter::Select(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	jint func = arCommand.GetCode();
	jshort count = arCommand.mCount;
	jlong ontime = arCommand.mOnTimeMS;
	jlong offtime = arCommand.mOffTimeMS;
	jint status = arCommand.mStatus;
	jlong index = aIndex;

	jint ret = this->GetEnv()->CallIntMethod(mProxy, mSelectCROB, func, count, ontime, offtime, status, index);
	return IntToCommandStatus(ret);
}

CommandStatus CommandHandlerAdapter::Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	jint func = arCommand.GetCode();
	jshort count = arCommand.mCount;
	jlong ontime = arCommand.mOnTimeMS;
	jlong offtime = arCommand.mOffTimeMS;
	jint status = arCommand.mStatus;
	jlong index = aIndex;

	jint ret = this->GetEnv()->CallIntMethod(mProxy, mOperateCROB, func, count, ontime, offtime, status, index);
	return IntToCommandStatus(ret);
}

CommandStatus CommandHandlerAdapter::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	jint func = arCommand.GetCode();
	jshort count = arCommand.mCount;
	jlong ontime = arCommand.mOnTimeMS;
	jlong offtime = arCommand.mOffTimeMS;
	jint status = arCommand.mStatus;
	jlong index = aIndex;

	jint ret = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateCROB, func, count, ontime, offtime, status, index);
	return IntToCommandStatus(ret);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	jshort value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogInt16, value, index);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	jshort value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogInt16, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	jshort value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogInt16, value, index);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	jshort value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogInt32, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	jint value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogInt32, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	jint value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogInt32, value, index);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	jfloat value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogFloat32, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	jfloat value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogFloat32, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	jfloat value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogFloat32, value, index);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	jdouble value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogDouble64, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	jdouble value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogDouble64, value, index);
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	jdouble value = arCommand.GetValue();
	jlong index = aIndex;

	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogDouble64, value, index);
	return IntToCommandStatus(status);
}

JNIEnv* CommandHandlerAdapter::GetEnv()
{
	return JNIHelpers::GetEnvFromJVM(mpJVM);
}





