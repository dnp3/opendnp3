using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class ActionNodeFactory : ISimulatorNodeFactory
    {
        readonly string displayName;
        readonly Func<ISimulatorNodeCallbacks, ISimulatorNode> factory;


        public ActionNodeFactory(string displayName, Func<ISimulatorNodeCallbacks, ISimulatorNode> factory)
        {
            this.displayName = displayName;
            this.factory = factory;
        }

        string ISimulatorNodeFactory.DisplayName
        {
            get { return displayName; }
        }

        ISimulatorNode ISimulatorNodeFactory.Create(ISimulatorNodeCallbacks callbacks)
        {
            return factory.Invoke(callbacks);
        }
    }
}
