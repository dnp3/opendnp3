using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class ChannelNode : ISimulatorNode
    {
        readonly IChannel channel;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;

        public ChannelNode(IChannel channel, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.channel = channel;
            this.callbacks = callbacks;
            this.alias = alias;
        }

        void ISimulatorNode.Remove()
        {
            channel.Shutdown();
        }

        IEnumerable<Metric> ISimulatorNode.GetMetrics()
        {
            return Enumerable.Empty<Metric>();
        }

        string ISimulatorNode.DisplayName
        {
            get { return alias; }
        }
    }
}
