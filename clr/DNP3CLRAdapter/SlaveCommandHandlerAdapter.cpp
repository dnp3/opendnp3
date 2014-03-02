#include "SlaveCommandHandlerAdapter.h"
#include "Conversions.h"



namespace DNP3
{
namespace Adapter
{

SlaveCommandHandlerAdapter::SlaveCommandHandlerAdapter(DNP3::Interface::ICommandHandler ^ proxy) : proxy(proxy)
{

}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Supports(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Perform(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

}
}

