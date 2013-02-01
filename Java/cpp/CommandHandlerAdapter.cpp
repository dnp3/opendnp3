
#include "CommandHandlerAdapter.h"
#include "JNIHelpers.h"

using namespace apl::dnp;

CommandHandlerAdapter::CommandHandlerAdapter(JavaVM* apJVM, jobject aProxy) : mpJVM(apJVM), mProxy(aProxy)
{}

CommandStatus CommandHandlerAdapter::Select(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence)
{
	return CS_SUCCESS;
/*
	auto pEnv = GetEnv();
	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);
	jmethodid mid = pEnv->GetMethodId(clazz, "Select", "(ISJJIJ)I");
	assert(mid != NULL);
	//int function, short count, long onTimeMs, long offTimeMs, int status, long index)
	jint func = arCommand
	jint status = pEnv->CallIntMethod(mProxy, mid, arCommand.mFuncit 
	*/
}

CommandStatus CommandHandlerAdapter::Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{ return CS_SUCCESS; }

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex)
{ return CS_SUCCESS; }

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex)
{ return CS_SUCCESS; }

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex)
{ return CS_SUCCESS; }

CommandStatus CommandHandlerAdapter::Select(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::Operate(const AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence)
{ return CS_SUCCESS; }
CommandStatus CommandHandlerAdapter::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex)
{ return CS_SUCCESS; }
	
JNIEnv* CommandHandlerAdapter::GetEnv()
{
	return JNIHelpers::GetEnvFromJVM(mpJVM);
}





