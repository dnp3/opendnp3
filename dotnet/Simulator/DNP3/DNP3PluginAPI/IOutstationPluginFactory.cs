using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3.API
{   
    public interface IOutstationModule
    {
        OutstationStackConfig DefaultConfig
        {
            get;
        }

        bool AllowTemplateEditing
        {
            get;
        }

        string Description
        {
            get;
        }

        string Name
        {
            get;
        }

        string DefaultLogName
        {
            get;
        }

        IOutstationFactory CreateFactory();
        
    }

    public interface IOutstationInstance
    {
        bool HasForm
        {
            get;
        }

        bool ShowFormOnCreation
        {
            get;
        }

        string DisplayName
        {
            get;
        }

        void ShowForm();

        void Shutdown();        
    }

    public interface IOutstationFactory
    {
        ICommandHandler CommandHandler
        {
            get;
        }

        IOutstationApplication Application
        {
            get;
        }

        IOutstationInstance CreateInstance(IOutstation outstation, string name, OutstationStackConfig config);
    }


}
