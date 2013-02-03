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
