using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;
using Automatak.Simulator.DNP3.API;

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{
    public class OutstationModule : IOutstationModule
    {
        OutstationStackConfig IOutstationModule.DefaultConfig
        {
            get {
                return new OutstationStackConfig();
            }
        }

        bool IOutstationModule.AllowTemplateEditing
        {
            get { return true; }
        }

        IOutstationFactory IOutstationModule.CreateFactory()
        {
            throw new NotImplementedException();
        }

        public IOutstationModule Instance
        {
            get
            {
                return instance;
            }
        }
       
        private OutstationModule()
        {}

        private static OutstationModule instance = new OutstationModule();
    }


    class OutstationFactory : IOutstationFactory
    {
        private readonly ProxyCommandHandler commandHandler = new ProxyCommandHandler();
        private readonly EventedOutstationApplication application = new EventedOutstationApplication();


        ICommandHandler IOutstationFactory.CommandHandler
        {
            get { return commandHandler; }
        }

        IOutstationApplication IOutstationFactory.Application
        {
            get { return application; }
        }

        IOutstationInstance IOutstationFactory.Create(IOutstation outstation, string name, OutstationStackConfig config)
        {
            throw new NotImplementedException();
        }
    }

    
    class OutstationInstance : IOutstationInstance
    {
        /*
        readonly MeasurementCache cache;
        readonly ProxyCommandHandler handler;
        readonly EventedOutstationApplication application;
        readonly IOutstation outstation;        
        readonly string alias;
        */


        //OutstationForm form = null;
        /*
        string ISimulatorNode.Alias
        {
            get
            {
                return alias;
            }
        }

        public OutstationNode(MeasurementCache cache, ProxyCommandHandler handler, EventedOutstationApplication application, IDNP3Config config, IOutstation outstation, ISimulatorNodeCallbacks callbacks, string alias)
        {
            this.cache = cache;
            this.handler = handler;
            this.application = application;
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
                form = new OutstationForm(outstation, application, cache, handler, alias);
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
        */
        bool IOutstationInstance.HasForm
        {
            get { throw new NotImplementedException(); }
        }

        void IOutstationInstance.ShowForm()
        {
            throw new NotImplementedException();
        }

        void IOutstationInstance.Shutdown()
        {
            throw new NotImplementedException();
        }

        IEnumerable<Metric> IOutstationInstance.Metrics
        {
            get { throw new NotImplementedException(); }
        }
    }
    
}
