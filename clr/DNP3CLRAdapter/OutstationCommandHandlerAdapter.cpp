#include "OutstationCommandHandlerAdapter.h"
#include "Conversions.h"



namespace DNP3
{
namespace Adapter
{

OutstationCommandHandlerAdapter::OutstationCommandHandlerAdapter(DNP3::Interface::ICommandHandler^ proxy) : proxy(proxy)
{

}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Supports(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Supports(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Supports(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Perform(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus OutstationCommandHandlerAdapter::Perform(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Perform(Conversions::convertCommand(arCommand), aIndex));
}

}
}

