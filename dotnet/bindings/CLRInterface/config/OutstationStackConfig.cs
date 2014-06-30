using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Aggreate configuration for an outstation stack
    /// </summary>
    public class OutstationStackConfig
    {
        /// <summary>
        /// Constructor with reasonable defaults
        /// </summary>
        public OutstationStackConfig()
        {
            this.outstation = new OutstationConfig();
            this.buffer = new EventBufferConfig();
            this.databaseTemplate = new DatabaseTemplate(10, 10, 10, 10, 10, 10, 10);
            this.link = new LinkConfig(false, false);
        }

        /// <summary>
        /// Default variations to use in static responses
        /// </summary>
        public StaticResponseConfig defaultStaticResponses = new StaticResponseConfig();

        /// <summary>
        /// Default variations to use in event responses
        /// </summary>
        public EventResponseConfig defaultEventResponses = new EventResponseConfig();

        /// <summary>
        /// Outstation config
        /// </summary>
        public OutstationConfig outstation;

        /// <summary>
        /// Configuration of the outstation event buffer
        /// </summary>
        public EventBufferConfig buffer;

        /// <summary>
        /// Device template that specifies database layout, control behavior
        /// </summary>
        public DatabaseTemplate databaseTemplate;

        /// <summary>
        /// Link layer config
        /// </summary>
        public LinkConfig link;
    }

}
