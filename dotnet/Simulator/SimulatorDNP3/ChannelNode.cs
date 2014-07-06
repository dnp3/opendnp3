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
            
            this.callbacks.ChangeImage(IconIndex.Channel);

            this.masterFactory = new ActionNodeFactory("Add Master", cb => CreateMaster(cb));


            this.channel.AddStateListener(state => callbacks.ChangeState(GetNodeState(state)));
        }

        
        NodeState GetNodeState(ChannelState state)
        {
            switch (state)
            { 
                case(ChannelState.OPENING):
                    return NodeState.Waiting;
                case (ChannelState.WAITING):
                    return NodeState.Failed;
                case (ChannelState.OPEN):
                    return NodeState.Active;                
                default:
                    return NodeState.Inactive;
            }
        }        

        ISimulatorNode CreateMaster(ISimulatorNodeCallbacks callbacks)
        {
            using (var dialog = new Components.MasterDialog())
            {
                dialog.ShowDialog();
                if (dialog.DialogResult == DialogResult.OK)
                {
                    var form = new GUIMasterForm();
                    var config = dialog.Configuration;
                    var master = channel.AddMaster("master", form.SequenceOfEvents, DefaultMasterApplication.Instance, config);

                    if (master == null)
                    {
                        return null;
                    }
                    else
                    {
                        form.SetMaster(master);
                        master.Enable();
                        return new MasterNode(form, master, callbacks, "master");
                    }                    
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

        IEnumerable<Metric> ISimulatorNode.Metrics
        {
            get
            {
                var list = new List<Metric>();
                var stats = channel.GetChannelStatistics();
                list.Add(new Metric("bytes rx", stats.NumBytesRx.ToString()));
                list.Add(new Metric("bytes tx", stats.NumBytesTx.ToString()));
                list.Add(new Metric("crc errors", stats.NumCrcError.ToString()));
                list.Add(new Metric("open count", stats.NumOpen.ToString()));
                list.Add(new Metric("num close", stats.NumClose.ToString()));
                list.Add(new Metric("open fail count", stats.NumOpenFail.ToString()));
                list.Add(new Metric("link frames rx", stats.NumLinkFrameRx.ToString()));
                list.Add(new Metric("link frames tx", stats.NumLinkFrameTx.ToString()));
                list.Add(new Metric("bad link frames rx", stats.NumBadLinkFrameRx.ToString()));
                return list;
            }
        }

        string ISimulatorNode.DisplayName
        {
            get { return alias; }
        }

        IEnumerable<ISimulatorNodeAction> ISimulatorNode.Actions
        {
            get
            {
                return Enumerable.Empty<ISimulatorNodeAction>();
            }
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
