using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Class that defines how many events an outstation will before losing events
    /// </summary>
    public class EventBufferConfig
    {
        /// <summary>
        /// All events set to same count
        /// </summary>
        public EventBufferConfig(UInt16 count)
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
        public System.UInt16 maxBinaryEvents;

        /// <summary>
        /// The number of double-bit binary events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt16 maxDoubleBinaryEvents;

        /// <summary>
        /// The number of analog events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt16 maxAnalogEvents;

        /// <summary>
        /// The number of counter events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt16 maxCounterEvents;

        /// <summary>
        /// The number of frozen counter events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt16 maxFrozenCounterEvents;

        /// <summary>
        /// The number of binary output status events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt16 maxBinaryOutputStatusEvents;

        /// <summary>
        /// The number of analog output status events the outstation will buffer before overflowing
        /// </summary>
        public System.UInt16 maxAnalogOutputStatusEvents;
    }

}
