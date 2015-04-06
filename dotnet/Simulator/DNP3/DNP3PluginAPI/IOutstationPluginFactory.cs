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

        IOutstationFactory CreateFactory();
        
    }

    public interface IOutstationInstance
    {
        bool HasForm
        {
            get;
        }

        void ShowForm();

        void Shutdown();

        IEnumerable<Metric> Metrics
        {
            get;
        }
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

        IOutstationInstance Create(IOutstation outstation, string name, OutstationStackConfig config);
    }


}
