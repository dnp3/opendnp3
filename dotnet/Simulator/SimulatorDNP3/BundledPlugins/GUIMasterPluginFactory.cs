using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class GUIMasterPluginFactory : IMasterPluginFactory
    {
        private GUIMasterPluginFactory() {}

        private static IMasterPluginFactory instance = new GUIMasterPluginFactory();

        public static IMasterPluginFactory Instance
        {
            get
            {
                return instance;
            }
        }

        MasterStackConfig IMasterPluginFactory.DefaultConfiguration
        {
            get { return new MasterStackConfig(); }
        }

        string IMasterPluginFactory.Name
        {
            get { return "Master GUI"; }
        }

        IMasterPlugin IMasterPluginFactory.Create(string id)
        {
            return new GUIMasterPlugin();
        }
    }
}
