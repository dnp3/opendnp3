#include <opendnp3/SimpleCommandHandler.h>

namespace opendnp3
{

SimpleCommandHandler::SimpleCommandHandler(std::function<CommandStatus ()> aStatusFunc) :
	mStatusFunc(aStatusFunc)
{

}

CommandStatus SimpleCommandHandler::Select(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return mStatusFunc();
}

CommandStatus SimpleCommandHandler::Select(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::Operate(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex)
{
	return mStatusFunc();
}

CommandStatus SimpleCommandHandler::Select(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::Operate(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex)
{
	return mStatusFunc();
}

CommandStatus SimpleCommandHandler::Select(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::Operate(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return mStatusFunc();
}

CommandStatus SimpleCommandHandler::Select(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::Operate(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return mStatusFunc();
}
CommandStatus SimpleCommandHandler::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return mStatusFunc();
}

SuccessCommandHandler::SuccessCommandHandler() : SimpleCommandHandler([]()
{
	return CommandStatus::SUCCESS;
})
{

}

SuccessCommandHandler SuccessCommandHandler::mHandler;

}


