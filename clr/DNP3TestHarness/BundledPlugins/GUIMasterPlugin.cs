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

        public ISOEHandler SOEHandler
        {
            get { return form.SequenceOfEvents; }
        }
        
        public void SetMaster(IMaster master)
        {
            
        }

        public System.Windows.Forms.Form PluginForm
        {
            get
            {
                return form;
            }
        }

        public void Shutdown()
        {
            
        }
    }
}
