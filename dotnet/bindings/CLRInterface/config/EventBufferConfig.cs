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
        /// All events set to same count
        /// </summary>
        public EventBufferConfig(uint count)
        {
            this.maxBinaryEvents = count;
            this.maxDoubleBinaryEvents = count;
            this.maxAnalogEvents = count;
            this.maxCounterEvents = count;
            this.maxFrozenCounterEvents = count;
            this.maxBinaryOutputStatusEvents = count;
            this.maxAnalogOutputStatusEvents = count;
        }


        /// <summary>
        /// All events set to 100
        /// </summary>
        public EventBufferConfig() : this(100)
        {
           
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
