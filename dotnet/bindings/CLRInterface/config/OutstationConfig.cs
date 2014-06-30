using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Includes the OutstationParams and the default response settings
    /// </summary>
    public class OutstationConfig
    {      
        OutstationParams config = new OutstationParams();

        StaticResponseConfig staticConfig = new StaticResponseConfig();

        EventResponseConfig eventConfig = new EventResponseConfig();         
    }  
}
