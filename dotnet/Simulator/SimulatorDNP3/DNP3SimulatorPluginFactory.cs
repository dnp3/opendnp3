using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    public class DNP3SimulatorPluginFactory : ISimulatorPluginFactory
    {
        ISimulatorPlugin ISimulatorPluginFactory.Create()
        {
            return new DNP3SimulatorPlugin();
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
