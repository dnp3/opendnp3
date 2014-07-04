
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public interface IMasterPluginFactory
    {
        MasterStackConfig DefaultConfiguration
        {
            get;
        }

        string Name
        {
            get;
        }

        IMasterPlugin Create(string id);
    }
}
