#include "SlaveCommandHandlerAdapter.h"
#include "Conversions.h"



namespace DNP3
{
namespace Adapter
{

SlaveCommandHandlerAdapter::SlaveCommandHandlerAdapter(DNP3::Interface::ICommandHandler ^ proxy) : proxy(proxy)
{

}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}





}
}

