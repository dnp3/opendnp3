using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
    public interface ICommandAcceptor
    {
        IFuture<CommandStatus> AcceptCommand(BinaryOutput command, System.UInt32 index);
        IFuture<CommandStatus> AcceptCommand(Setpoint command, System.UInt32 index);
    }
}
