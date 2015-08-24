using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

using Automatak.Simulator.API;
using Automatak.Simulator.DNP3.API;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.RelayOutstationPlugin
{
    public class OutstationModule : IOutstationModule
    {
        const string name = "Relay Simulator";

        OutstationStackConfig IOutstationModule.DefaultConfig
        {
            get 
            {
                var config =  new OutstationStackConfig();
                config.databaseTemplate = new DatabaseTemplate(1, 0, 6, 0, 0, 0, 0, 0);
                return config;
            }
        }

        bool IOutstationModule.AllowTemplateEditing
        {
            get { return false; }
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
            get { return "A simple relay simulator with a mock front panel. Responds to latch on/off"; }
        }

        string IOutstationModule.Name
        {
            get { return name; }
        }


        string IOutstationModule.DefaultLogName
        {
            get { return "relay"; }
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
        readonly OutstationForm form;
        
        public OutstationInstance(ProxyCommandHandler handler, EventedOutstationApplication application, IOutstation outstation, OutstationStackConfig config, string alias)
        {
            this.handler = handler;
            this.application = application;
            this.outstation = outstation;
            this.alias = alias;

            this.form = new OutstationForm(outstation, handler);
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
                return true;
            }
        }

        void IOutstationInstance.ShowForm()
        {            
            form.Show();
        }

        void IOutstationInstance.Shutdown()
        {                        
            form.Close();
            form.Dispose();                            
        }
    }
    
}
