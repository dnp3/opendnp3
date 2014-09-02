#ifndef __SLAVE_COMMAND_HANDLER_ADAPTER_H_
#define __SLAVE_COMMAND_HANDLER_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <opendnp3/outstation/ICommandHandler.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			//this object goes into the stack
			private class OutstationCommandHandlerAdapter : public opendnp3::ICommandHandler
			{
			public:
				OutstationCommandHandlerAdapter(Automatak::DNP3::Interface::ICommandHandler^ proxy);

				opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex) override final;

				opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& arCommand, uint16_t aIndex) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& arCommand, uint16_t aIndex) override final;


			private:
				gcroot < Automatak::DNP3::Interface::ICommandHandler^ > proxy;
			};

		}
	}
}

#endif
