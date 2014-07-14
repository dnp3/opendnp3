using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3
{
    class MasterNode : ISimulatorNode
    {
        readonly IDNP3Config config;
        readonly MeasurementCache cache;
        readonly IMaster master;
        readonly ISimulatorNodeCallbacks callbacks;
        readonly string alias;
        readonly ISimulatorNodeAction openAction;

        readonly Guid guid = new Guid();

        MasterForm form = null;

        string ISimulatorNode.Alias
        {
            get
            {
                return alias;
            }
        }

        public MasterNode(IDNP3Config config, MeasurementCache cache, IMaster master, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.config = config;
            this.cache = cache;
            this.master = master;
            this.callbacks = callbacks;
            this.alias = alias;

            this.callbacks.ChangeImage(IconIndex.Master);

            this.openAction = new NodeAction("Open", () => OpenForm());
        }

        void OpenForm()
        {
            if (form == null)
            {
                form = new MasterForm(master, cache, alias);
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


        Guid ISimulatorNode.UniqueID
        {
            get { return guid; }
        }
    }
}
