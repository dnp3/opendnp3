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
            var manager = DNP3SessionManagerFactory.CreateManager(4, new PrintingLogAdapter());

            var listener = manager.CreateListener("listener", LogLevels.ALL, IPEndpoint.Localhost(20000), new DefaultListenCallbacks());

            Console.WriteLine("Enter a command");

            while (true)
            {
                switch (Console.ReadLine())
                {                    
                    case "x":                        
                        // The manager does not automatically reclaim resources for stop executing threads in a finalizer
                        // Be sure to call this before exiting to cleanup all resources.
                        manager.Shutdown();
                        return 0;
                    default:
                        break;
                }
            }
        }
    }
}
