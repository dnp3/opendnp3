using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class OutstationNode : ISimulatorNode
    {
        readonly MeasurementCache cache;
        readonly ProxyCommandHandler handler;
        readonly IDNP3Config config;
        readonly IOutstation outstation;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;
        readonly ISimulatorNodeAction openAction;

        readonly Guid guid = new Guid();
        

        OutstationForm form = null;

        string ISimulatorNode.Alias
        {
            get
            {
                return alias;
            }
        }

        public OutstationNode(MeasurementCache cache, ProxyCommandHandler handler, IDNP3Config config, IOutstation outstation, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.cache = cache;
            this.handler = handler;
            this.config = config;
            this.outstation = outstation;
            this.callbacks = callbacks;
            this.alias = alias;

            this.callbacks.ChangeImage(IconIndex.Outstation);

            this.openAction = new NodeAction("Open", () => OpenForm());
        }

        void OpenForm()
        {
            if (form == null)
            {
                form = new OutstationForm(outstation, cache, handler, alias);
            }

            form.Show();
        }
        
        void ISimulatorNode.Remove()
        {
            if (form != null)
            {
                form.Close();
                form.Dispose();
                form = null;
            }
            outstation.Shutdown();            
        }

        IEnumerable<Metric> ISimulatorNode.Metrics
        {
            get
            {
                var list = new List<Metric>();
                var stats = outstation.GetStackStatistics();
                list.Add(new Metric("Num transport rx", stats.NumTransportRx.ToString()));
                list.Add(new Metric("Num transport tx", stats.NumTransportTx.ToString()));
                list.Add(new Metric("Num transport error rx", stats.NumTransportErrorRx.ToString()));
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
                yield return openAction;
            }
        }

        IEnumerable<ISimulatorNodeFactory> ISimulatorNode.Children
        {
            get { return Enumerable.Empty<ISimulatorNodeFactory>(); }
        }


        Guid ISimulatorNode.UniqueID
        {
            get { return guid; }
        }
    }
}
