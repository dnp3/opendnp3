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

	mSelectCROB = JNIHelpers::GetMethodID(pEnv, clazz, "Select",  "(ISJJIJB)I");
	mOperateCROB = JNIHelpers::GetMethodID(pEnv, clazz, "Operate",  "(ISJJIJB)I");
	mDirectOperateCROB = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate",  "(ISJJIJ)I"); 

	mSelectAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(SJB)I");
	mOperateAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(SJB)I");
	mDirectOperateAnalogInt16 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(SJ)I");

	mSelectAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(IJB)I");
	mOperateAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(IJB)I");
	mDirectOperateAnalogInt32 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(IJ)I");

	mSelectAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(FJB)I");
	mOperateAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(FJB)I");
	mDirectOperateAnalogFloat32 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(FJ)I");

	mSelectAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "Select", "(DJB)I");
	mOperateAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "Operate", "(DJB)I");
	mDirectOperateAnalogDouble64 = JNIHelpers::GetMethodID(pEnv, clazz, "DirectOperate", "(DJ)I");	

}

CommandStatus CommandHandlerAdapter::Select(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence)
{	
	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectCROB, arCommand.GetCode(), arCommand.mCount, arCommand.mOnTimeMS, arCommand.mOffTimeMS, arCommand.mStatus, aIndex, aSequence);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence)
{	
	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateCROB, arCommand.GetCode(), arCommand.mCount, arCommand.mOnTimeMS, arCommand.mOffTimeMS, arCommand.mStatus, aIndex, aSequence);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{	
	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateCROB, arCommand.GetCode(), arCommand.mCount, arCommand.mOnTimeMS, arCommand.mOffTimeMS, arCommand.mStatus, aIndex);
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogInt16, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogInt16, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogInt16, arCommand.GetValue(), aIndex); 
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogInt32, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogInt32, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogInt32, arCommand.GetValue(), aIndex); 
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogFloat32, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogFloat32, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogFloat32, arCommand.GetValue(), aIndex); 
	return IntToCommandStatus(status);
}

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mSelectAnalogDouble64, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mOperateAnalogDouble64, arCommand.GetValue(), aIndex, aSequence); 
	return IntToCommandStatus(status);
}
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	jint status = this->GetEnv()->CallIntMethod(mProxy, mDirectOperateAnalogDouble64, arCommand.GetValue(), aIndex); 
	return IntToCommandStatus(status);
}
	
JNIEnv* CommandHandlerAdapter::GetEnv()
{
	return JNIHelpers::GetEnvFromJVM(mpJVM);
}





