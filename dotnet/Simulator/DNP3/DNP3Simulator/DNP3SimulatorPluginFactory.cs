using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.Simulator.Commons;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    public class DNP3SimulatorPluginFactory : ISimulatorPluginFactory
    {
        ISimulatorPlugin ISimulatorPluginFactory.Create(ILog log)
        {
            return new DNP3SimulatorPlugin(log);
        }

        public static ISimulatorPluginFactory Instance
        {
            get
            {
                return instance;
            }
        }

        static ISimulatorPluginFactory instance = new DNP3SimulatorPluginFactory();

        DNP3SimulatorPluginFactory()
        {}
    }
}
