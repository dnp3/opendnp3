using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.DNP3.Adapter;

namespace DotNetMasterGPRSDemo
{
    class Program
    {
        static int Main(string[] args)
        {
            var manager = DNP3SessionManagerFactory.CreateManager(4, PrintingLogAdapter.Instance);

            var listener = manager.CreateListener("listener", LogLevels.ALL, IPEndpoint.Localhost(20000), null);

            Console.WriteLine("Enter a command");

            while (true)
            {
                switch (Console.ReadLine())
                {                    
                    case "x":
                        return 0;
                    default:
                        break;
                }
            }
        }
    }
}
