#ifndef __SLAVE_COMMAND_HANDLER_ADAPTER_H_
#define __SLAVE_COMMAND_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <opendnp3/ICommandHandler.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
//this object goes into the stack
private class SlaveCommandHandlerAdapter : public opendnp3::ICommandHandler
{
public:
	SlaveCommandHandlerAdapter(DNP3::Interface::ICommandHandler ^ proxy);

	opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex);
	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex);
	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex);
	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex);
	opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex);

	opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex);
	opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex);


	opendnp3::CommandStatus DirectOperate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex);
	opendnp3::CommandStatus DirectOperate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex);


private:
	gcroot < DNP3::Interface::ICommandHandler ^ > proxy;
};

private ref class SlaveCommandHandlerWrapper
{
public:

	SlaveCommandHandlerWrapper(DNP3::Interface::ICommandHandler ^ proxy) :
		mpAdapter(new SlaveCommandHandlerAdapter(proxy))
	{}

	~SlaveCommandHandlerWrapper() {
		delete mpAdapter;
	}

	opendnp3::ICommandHandler* Get() {
		return mpAdapter;
	}

private:
	SlaveCommandHandlerAdapter* mpAdapter;
};
}
}

#endif
