using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.API;

namespace Automatak.Simulator.DNP3.API
{   
    public interface IOutstationPluginFactory
    {
        OutstationStackConfig DefaultConfig
        {
            get;
        }

        bool AllowTemplateEditing
        {
            get;
        }

        IOutstationInstanceFactory Create();
        
    }

    public interface IOutstationInstanceFactory
    {
        ICommandHandler CommandHandler
        {
            get;
        }

        IOutstationApplication Application
        {
            get;
        }

        ISimulatorNode Create(IOutstation outstation, OutstationStackConfig config);
    }


}
