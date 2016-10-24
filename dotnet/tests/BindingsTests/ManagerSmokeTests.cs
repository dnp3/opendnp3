using System;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using Automatak.DNP3.Adapter;
using Automatak.DNP3.Interface;

namespace BindingsTests
{
    public class MyListener : IChannelListener
    {
        public void OnStateChange(ChannelState state)
        {
            Console.WriteLine("channel state: " + state);
        }
    }

    [TestClass]
    public class ManagerSmokeTests
    {
        /// <summary>
        /// https://github.com/automatak/dnp3/issues/168
        /// </summary>
        [TestMethod]
        public void TestMethod1()
        {
            IDNP3Manager mgr = DNP3ManagerFactory.CreateManager(1, new PrintingLogAdapter());
            IChannel channel = mgr.AddTCPServer("server", LogLevels.NORMAL, ChannelRetry.Default, "127.0.0.1", 20000, new MyListener());

            Thread.Sleep(20);
            Console.WriteLine(20);

            Thread.Sleep(50);
            Console.WriteLine(50);

            Thread.Sleep(50);
            Console.WriteLine(50);

            Thread.Sleep(100);
            Console.WriteLine(100);

            Thread.Sleep(1000);
            Console.WriteLine(1000);
        }
    }
}
