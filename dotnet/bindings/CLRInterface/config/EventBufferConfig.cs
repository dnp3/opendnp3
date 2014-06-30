using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Class that defines how many events an outstation will before losing events
    /// </summary>
    public class EventBufferConfig
    {

        /// <summary>
        /// All events set to 1000
        /// </summary>
        public EventBufferConfig()
        {
            this.maxBinaryEvents = 1000;
            this.maxDoubleBinaryEvents = 1000;
            this.maxAnalogEvents = 1000;
            this.maxCounterEvents = 1000;
            this.maxFrozenCounterEvents = 1000;
            this.maxBinaryOutputStatusEvents = 1000;
            this.maxAnalogOutputStatusEvents = 1000;
        }

        /// <summary>
        /// The number of binary events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxBinaryEvents;

        /// <summary>
        /// The number of double-bit binary events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxDoubleBinaryEvents;

        /// <summary>
        /// The number of analog events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxAnalogEvents;

        /// <summary>
        /// The number of counter events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxCounterEvents;

        /// <summary>
        /// The number of frozen counter events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxFrozenCounterEvents;

        /// <summary>
        /// The number of binary output status events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxBinaryOutputStatusEvents;

        /// <summary>
        /// The number of analog output status events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt32 maxAnalogOutputStatusEvents;
    }

}
