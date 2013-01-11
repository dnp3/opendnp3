using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Adapter;
using DNP3.Interface;

namespace DotNetSlaveDemo
{
    class RejectingCommandAcceptor : ICommandAcceptor {

        public IFuture<CommandStatus> AcceptCommand(BinaryOutput command, uint index)
        {
            System.Console.WriteLine("Received BinaryOutput on index: " + index);
            var future = new Future<CommandStatus>();
            future.Set(CommandStatus.CS_FORMAT_ERROR);
            return future;
        }

        public IFuture<CommandStatus> AcceptCommand(Setpoint command, uint index)
        {
            System.Console.WriteLine("Received Setpoint on index: " + index);
            var future = new Future<CommandStatus>();
            future.Set(CommandStatus.CS_NOT_SUPPORTED);
            return future;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            var sm = new StackManager();
            sm.AddTCPServer("server", FilterLevel.LEV_INFO, 5000, "127.0.0.1", 20000);
            var config = new SlaveStackConfig();
            var publisher = sm.AddSlave("server", "slave", FilterLevel.LEV_INFO, new RejectingCommandAcceptor(), config);

            Console.WriteLine("Press <Enter> to randomly change a value");

            Random r = new Random();
            while (true)
            {
                Console.ReadLine();
                int value = r.Next(UInt16.MaxValue);
                System.Console.WriteLine("Change Analog 0 to: " + value);
                publisher.Start();
                publisher.Update(new Analog(value, 1, DateTime.Now), 0);
                publisher.End();
            }
        }
    }
}
