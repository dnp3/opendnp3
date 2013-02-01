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

	JNIEnv* GetEnv();

	JavaVM* mpJVM;
	jobject mProxy;	
};

#endif
