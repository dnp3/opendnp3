using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;
using Automatak.Simulator.DNP3.API;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{
    public class OutstationModule : IOutstationModule
    {
        const string name = "DefaultOutstation";

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
            return new OutstationFactory();
        }

        public static IOutstationModule Instance
        {
            get
            {
                return instance;
            }
        }
       
        private OutstationModule()
        {}

        private static OutstationModule instance = new OutstationModule();

        public override string ToString()
        {
            return name;
        }

        string IOutstationModule.Description
        {
            get { return "A default outstation simulator that allows database editing and other behaviors"; }
        }

        string IOutstationModule.Name
        {
            get { return name; }
        }

        string IOutstationModule.DefaultLogName
        {
            get { return "outstation"; }
        }
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

        IOutstationInstance IOutstationFactory.CreateInstance(IOutstation outstation, string name, OutstationStackConfig config)
        {
            return new OutstationInstance(commandHandler, application, outstation, config, name);
        }
    }

    
    class OutstationInstance : IOutstationInstance
    {        
        readonly ProxyCommandHandler handler;
        readonly EventedOutstationApplication application;
        readonly IOutstation outstation;
        readonly string alias;
        readonly MeasurementCache cache;

        OutstationForm form = null;
        
        public OutstationInstance(ProxyCommandHandler handler, EventedOutstationApplication application, IOutstation outstation, OutstationStackConfig config, string alias)
        {
            this.handler = handler;
            this.application = application;
            this.outstation = outstation;
            this.alias = alias;

            this.cache = new MeasurementCache(config.databaseTemplate);
        }

        string IOutstationInstance.DisplayName
        {
            get { return alias; }
        }

        bool IOutstationInstance.HasForm
        {
            get { return true; }
        }

        bool IOutstationInstance.ShowFormOnCreation
        {
            get
            {
                return false;
            }
        }

        void IOutstationInstance.ShowForm()
        {
            if(this.form == null)
            {
                this.form = new OutstationForm(outstation, application, cache, handler, alias);
            }

            form.Show();
        }

        void IOutstationInstance.Shutdown()
        {            
            if (form != null)
            {
                form.Close();
                form.Dispose();
                form = null;
            }         
        }
    }
    
}
