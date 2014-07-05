using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class ChannelNode : ISimulatorNode
    {
        readonly IChannel channel;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;

        readonly ISimulatorNodeFactory masterFactory;

        public ChannelNode(IChannel channel, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.channel = channel;
            this.callbacks = callbacks;
            this.alias = alias;

            this.masterFactory = new ActionNodeFactory("Add Master", cb => CreateMaster(cb));
        }

        ISimulatorNode CreateMaster(ISimulatorNodeCallbacks callbacks)
        {
            using (var dialog = new Components.MasterDialog())
            {
                dialog.ShowDialog();
                if (dialog.DialogResult == DialogResult.OK)
                {
                    var config = dialog.Configuration;
                    var master = channel.AddMaster("master", PrintingSOEHandler.Instance, DefaultMasterApplication.Instance, config);
                    return new MasterNode(master, callbacks, "master");
                }
                else
                {
                    return null;
                }
            }   
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

        IEnumerable<ISimulatorNodeFactory> ISimulatorNode.Children
        {
            get 
            {
                yield return masterFactory;
            }
        }
    }
}
