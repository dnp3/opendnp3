using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class NodeAction : ISimulatorNodeAction
    {
        readonly string displayName;
        readonly Action action;


        public NodeAction(string displayName, Action action)
        {
            this.displayName = displayName;
            this.action = action;
        }

        string ISimulatorNodeAction.DisplayName
        {
            get { return displayName; }
        }

        void ISimulatorNodeAction.Invoke()
        {
            action.Invoke();
        }
    }
}
