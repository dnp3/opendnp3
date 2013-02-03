#ifndef __COMMAND_HANDLER_ADAPTER_H_
#define __COMMAND_HANDLER_ADAPTER_H_

#include <jni.h>
#include <DNP3/ICommandHandler.h>

class CommandHandlerAdapter : public apl::dnp::ICommandHandler
{
	public:
	CommandHandlerAdapter(JavaVM* apJVM, jobject aProxy);

	apl::dnp::CommandStatus Select(const apl::dnp::ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus Operate(const apl::dnp::ControlRelayOutputBlock& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus DirectOperate(const apl::dnp::ControlRelayOutputBlock& arCommand, size_t aIndex);

	apl::dnp::CommandStatus Select(const apl::dnp::AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus Operate(const apl::dnp::AnalogOutputInt16& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus DirectOperate(const apl::dnp::AnalogOutputInt16& arCommand, size_t aIndex);

	apl::dnp::CommandStatus Select(const apl::dnp::AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus Operate(const apl::dnp::AnalogOutputInt32& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus DirectOperate(const apl::dnp::AnalogOutputInt32& arCommand, size_t aIndex);

	apl::dnp::CommandStatus Select(const apl::dnp::AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus Operate(const apl::dnp::AnalogOutputFloat32& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus DirectOperate(const apl::dnp::AnalogOutputFloat32& arCommand, size_t aIndex);

	apl::dnp::CommandStatus Select(const apl::dnp::AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus Operate(const apl::dnp::AnalogOutputDouble64& arCommand, size_t aIndex, uint8_t aSequence);
	apl::dnp::CommandStatus DirectOperate(const apl::dnp::AnalogOutputDouble64& arCommand, size_t aIndex);
	
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
