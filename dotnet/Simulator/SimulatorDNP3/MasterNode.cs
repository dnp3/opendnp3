using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class MasterNode : ISimulatorNode
    {
        readonly IMaster master;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;

        public MasterNode(IMaster master, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.master = master;
            this.callbacks = callbacks;
            this.alias = alias;
            this.callbacks.ChangeImage(IconIndex.InactiveMaster);
        }

        void ISimulatorNode.Remove()
        {
            master.Shutdown();
        }

        IEnumerable<Metric> ISimulatorNode.GetMetrics()
        {
            return Enumerable.Empty<Metric>();
        }

        string ISimulatorNode.DisplayName
        {
            get { return alias; }
        }

        IEnumerable<ISimulatorNodeFactory> ISimulatorNode.Children
        {
            get { return Enumerable.Empty<ISimulatorNodeFactory>(); }
        }
    }
}
