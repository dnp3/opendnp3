#ifndef OPENDNP3CLR_SLAVE_COMMAND_HANDLER_ADAPTER_H
#define OPENDNP3CLR_SLAVE_COMMAND_HANDLER_ADAPTER_H

#include <opendnp3/outstation/ICommandHandler.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			//this object goes into the stack
			private class OutstationCommandHandlerAdapter : public opendnp3::ICommandHandler
			{
			protected:

				void Start() override final;
				void End() override final;

			public:

				OutstationCommandHandlerAdapter(Automatak::DNP3::Interface::ICommandHandler^ proxy);
				
				opendnp3::CommandStatus Select(const opendnp3::ControlRelayOutputBlock& command, uint16_t index) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt32& command, uint16_t index) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputInt16& command, uint16_t index) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputFloat32& command, uint16_t index) override final;
				opendnp3::CommandStatus Select(const opendnp3::AnalogOutputDouble64& command, uint16_t index) override final;

				opendnp3::CommandStatus Operate(const opendnp3::ControlRelayOutputBlock& command, uint16_t index, opendnp3::OperateType opType) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt32& command, uint16_t index, opendnp3::OperateType opType) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputInt16& command, uint16_t index, opendnp3::OperateType opType) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputFloat32& command, uint16_t index, opendnp3::OperateType opType) override final;
				opendnp3::CommandStatus Operate(const opendnp3::AnalogOutputDouble64& command, uint16_t index, opendnp3::OperateType opType) override final;

			private:

				gcroot < Automatak::DNP3::Interface::ICommandHandler^ > proxy;
			};

		}
	}
}

#endif
