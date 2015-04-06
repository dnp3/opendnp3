using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

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

        IOutstationInstance Create(IOutstation outstation, OutstationStackConfig config);
    }


}
