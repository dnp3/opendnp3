#ifndef __SIMPLE_COMMAND_HANDLER_H_
#define __SIMPLE_COMMAND_HANDLER_H_

#include "ICommandHandler.h"

#include <functional>

namespace opendnp3
{

/**
* Mock ICommandHandler used for examples and demos
*/
class SimpleCommandHandler : public ICommandHandler
{
public:

	/**
	* @param aStatusFunc functor used to retrieve that next CommandStatus enumeration
	*/
	SimpleCommandHandler(std::function<CommandStatus ()> aStatusFunc);

	CommandStatus Select(const ControlRelayOutputBlock& arCommand, size_t aIndex);
	CommandStatus Operate(const ControlRelayOutputBlock& arCommand, size_t aIndex);
	CommandStatus DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex);

	CommandStatus Select(const AnalogOutputInt16& arCommand, size_t aIndex);
	CommandStatus Operate(const AnalogOutputInt16& arCommand, size_t aIndex);
	CommandStatus DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex);

	CommandStatus Select(const AnalogOutputInt32& arCommand, size_t aIndex);
	CommandStatus Operate(const AnalogOutputInt32& arCommand, size_t aIndex);
	CommandStatus DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex);

	CommandStatus Select(const AnalogOutputFloat32& arCommand, size_t aIndex);
	CommandStatus Operate(const AnalogOutputFloat32& arCommand, size_t aIndex);
	CommandStatus DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex);

	CommandStatus Select(const AnalogOutputDouble64& arCommand, size_t aIndex);
	CommandStatus Operate(const AnalogOutputDouble64& arCommand, size_t aIndex);
	CommandStatus DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex);

private:
	std::function<CommandStatus ()> mStatusFunc;
};

class SuccessCommandHandler : public SimpleCommandHandler
{
public:
	static SuccessCommandHandler* Inst() {
		return &mHandler;
	}

private:
	static SuccessCommandHandler mHandler;

protected:
	SuccessCommandHandler();
};

}

#endif

