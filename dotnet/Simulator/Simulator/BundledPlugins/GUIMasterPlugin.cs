using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    class GUIMasterPlugin : IMasterPlugin
    {
        private readonly GUIMasterForm form;

        public GUIMasterPlugin()
        {
            form = new GUIMasterForm();
        }                     

        ISOEHandler IMasterPlugin.SOEHandler
        {
            get { return form.SequenceOfEvents; }
        }

        System.Windows.Forms.Form IMasterPlugin.PluginForm
        {
            get
            {
                return form;
            }
        }

        void IMasterPlugin.SetMaster(IMaster master)
        {
            form.SetMaster(master);
        }

        void IMasterPlugin.Shutdown()
        {
            
        }
    }
}
