
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
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
