using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public interface IMasterPlugin
    {       
        ISOEHandler SOEHandler
        {
            get;
        }

        System.Windows.Forms.Form PluginForm
        {
            get;
        }

        void SetMaster(IMaster master);

        void Shutdown();
    }
}
