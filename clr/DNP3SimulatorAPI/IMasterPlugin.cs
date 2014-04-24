using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
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
