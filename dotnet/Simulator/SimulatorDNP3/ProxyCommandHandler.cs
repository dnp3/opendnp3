using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class ProxyCommandHandler : ICommandHandler
    {
        readonly Object mutex = new Object();
        ICommandHandler proxy = RejectingCommandHandler.Instance;

        ICommandHandler CommandHandler
        {
            get
            {
                lock (mutex)
                {
                    return proxy;
                }
            }
            set
            {
                lock (mutex)
                {
                    if (value == null)
                    {
                        proxy = RejectingCommandHandler.Instance;
                    }
                    else
                    {
                        proxy = value;
                    }
                }
            }
        }

        CommandStatus ICommandHandler.Supports(ControlRelayOutputBlock command, uint index)
        {
            return CommandHandler.Supports(command, index);
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputInt32 command, uint index)
        {
            return CommandHandler.Supports(command, index);
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputInt16 command, uint index)
        {
            return CommandHandler.Supports(command, index);
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputFloat32 command, uint index)
        {
            return CommandHandler.Supports(command, index);
        }

        CommandStatus ICommandHandler.Supports(AnalogOutputDouble64 command, uint index)
        {
            return CommandHandler.Supports(command, index);
        }

        CommandStatus ICommandHandler.Perform(ControlRelayOutputBlock command, uint index)
        {
            return CommandHandler.Perform(command, index);
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputInt32 command, uint index)
        {
            return CommandHandler.Perform(command, index);
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputInt16 command, uint index)
        {
            return CommandHandler.Perform(command, index);
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputFloat32 command, uint index)
        {
            return CommandHandler.Perform(command, index);
        }

        CommandStatus ICommandHandler.Perform(AnalogOutputDouble64 command, uint index)
        {
            return CommandHandler.Perform(command, index);
        }
    }
}
