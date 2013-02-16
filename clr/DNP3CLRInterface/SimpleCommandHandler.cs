using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Singeton that rejects all commands
    /// </summary>
    public class RejectingCommandHandler : SimpleCommandHandler
    {
        private static readonly ICommandHandler instance = new RejectingCommandHandler();

        private RejectingCommandHandler() : base(() => CommandStatus.CS_NOT_SUPPORTED)
        {}

        public static ICommandHandler Instance
        {
            get
            {
                return instance;
            }
        }
    }

    /// <summary>
    /// Handles all commands by calling a function
    /// </summary>
    public class SimpleCommandHandler : ICommandHandler
    {
        private readonly Func<CommandStatus> status;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="status">Function to call for return value</param>
        public SimpleCommandHandler(Func<CommandStatus> status)
        {
            this.status = status;
        }

        CommandStatus ICommandHandler.Select(ControlRelayOutputBlock command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Select(AnalogOutputInt32 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Select(AnalogOutputInt16 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Select(AnalogOutputFloat32 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Select(AnalogOutputDouble64 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Operate(ControlRelayOutputBlock command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputInt32 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputInt16 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputFloat32 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.Operate(AnalogOutputDouble64 command, uint index, byte aSequence)
        {
            return status();
        }

        CommandStatus ICommandHandler.DirectOperate(ControlRelayOutputBlock command, uint index)
        {
            return status();
        }

        CommandStatus ICommandHandler.DirectOperate(AnalogOutputInt32 command, uint index)
        {
            return status();
        }

        CommandStatus ICommandHandler.DirectOperate(AnalogOutputInt16 command, uint index)
        {
            return status();
        }

        CommandStatus ICommandHandler.DirectOperate(AnalogOutputFloat32 command, uint index)
        {
            return status();
        }

        CommandStatus ICommandHandler.DirectOperate(AnalogOutputDouble64 command, uint index)
        {
            return status();
        }
    }
}
