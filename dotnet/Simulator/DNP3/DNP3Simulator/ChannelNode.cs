using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;
using Automatak.Simulator.DNP3.API;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3
{
    class ChannelNode : ISimulatorNode
    {
        readonly IDNP3Config config;
        readonly IChannel channel;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;        

        readonly ISimulatorNodeFactory masterFactory;
        readonly ISimulatorNodeFactory outstationFactory;

        string ISimulatorNode.Alias
        {
            get
            {
                return alias;
            }
        }

        public ChannelNode(IDNP3Config config, IChannel channel, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.config = config;
            this.channel = channel;
            this.callbacks = callbacks;
            this.alias = alias;
            
            this.callbacks.ChangeImage(IconIndex.Channel);

            this.masterFactory = new ActionNodeFactory("Add Master", cb => CreateMaster(cb));
            this.outstationFactory = new ActionNodeFactory("Add Outstation", cb => CreateOutstation(cb));


            this.channel.AddStateListener(state => callbacks.ChangeState(GetNodeState(state)));
        }

        
        static NodeState GetNodeState(ChannelState state)
        {
            switch (state)
            { 
                case(ChannelState.OPENING):
                    return NodeState.Inactive;
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
                    var cache = new MeasurementCache();
                    var masterConfig = dialog.Configuration;
                    var alias = dialog.SelectedAlias;
                    var master = channel.AddMaster(alias, cache, DefaultMasterApplication.Instance, masterConfig);

                    if (master == null)
                    {
                        return null;
                    }
                    else
                    {                        
                        master.Enable();
                        return new MasterNode(cache, master, callbacks, alias);
                    }                    
                }
                else
                {
                    return null;
                }
            }   
        }

        ISimulatorNode CreateOutstation(ISimulatorNodeCallbacks callbacks)
        {
            using (var dialogModules = new Components.OutstationModuleDialog(config.OutstationModules))
            {
                if (dialogModules.ShowDialog() == DialogResult.OK)
                {                    
                    return CreateOutstation(callbacks, dialogModules.SelectedModule);                   
                }
                else
                {
                    return null;
                }
            }            
        }

        ISimulatorNode CreateOutstation(ISimulatorNodeCallbacks callbacks, IOutstationModule module)
        {
            using (var dialog = new Components.OutstationDialog(config, module))
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    var outstationConfig = dialog.Configuration;
                    var alias = dialog.SelectedAlias;
                    var factory = module.CreateFactory();

                    var outstation = channel.AddOutstation(alias, factory.CommandHandler, factory.Application, outstationConfig);

                    if (outstation == null)
                    {
                        return null;
                    }
                    else
                    {
                        var instance = factory.CreateInstance(outstation, alias, outstationConfig);
                        outstation.Enable();

                        if (instance.ShowFormOnCreation)
                        {
                            instance.ShowForm();
                        }

                        return new OutstationNode(outstation, instance, callbacks);
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
                yield return outstationFactory;
            }
        }
    }
}
