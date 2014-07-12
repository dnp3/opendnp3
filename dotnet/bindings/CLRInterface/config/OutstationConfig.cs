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
        /// <summary>
        /// Operational settings for the outstation
        /// </summary>
        public OutstationParams config = new OutstationParams();

        /// <summary>
        /// Configuration of the outstation event buffer
        /// </summary>
        public EventBufferConfig buffer = new EventBufferConfig();

        /// <summary>
        /// Default static responses
        /// </summary>
        public StaticResponseConfig staticConfig = new StaticResponseConfig();

        /// <summary>
        /// Default event responses
        /// </summary>
        public EventResponseConfig eventConfig = new EventResponseConfig();         
    }  
}
