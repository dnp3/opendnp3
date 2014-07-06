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
        readonly GUIMasterForm form;
        readonly IMaster master;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;
        readonly ISimulatorNodeAction openAction;

        public MasterNode(GUIMasterForm form, IMaster master, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.form = form;
            this.master = master;
            this.callbacks = callbacks;
            this.alias = alias;

            this.callbacks.ChangeImage(IconIndex.Master);

            this.openAction = new NodeAction("Open", () => form.Show());
        }

        void ISimulatorNode.Remove()
        {
            form.Close();
            master.Shutdown();
        }

        IEnumerable<Metric> ISimulatorNode.Metrics
        {
            get
            {
                var list = new List<Metric>();
                var stats = master.GetStackStatistics();
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
    }
}
