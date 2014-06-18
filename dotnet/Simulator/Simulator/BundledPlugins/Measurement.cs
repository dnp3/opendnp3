using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    enum MeasType
    {
        Binary,
        DoubleBitBinary,
        Counter,
        FrozenCounter,
        Analog,
        BinaryOutputStatus,
        AnalogOutputStatus,
        OctetString
    };

    class Measurement
    {
        public Measurement(string sValue, MeasurementBase meas, MeasType type, UInt16 index)
        {
            this.sValue = sValue;            
            this.flags = meas.Quality.ToString("X2");
            this.timeStamp = meas.Timestamp;            
            this.type = type;
            this.index = index;
        }

        public string EventString()
        {
            return String.Format("{0} - [{1}] -> {2} 0x{3}", type.ToString(), index, sValue, flags);
        }

        public ushort Index
        {
            get
            {
                return index;
            }
        }

        public string Value
        {
            get
            {
                return sValue;
            }
        }

        public string Flags
        {
            get
            {
                return flags;
            }
        }

        public DateTime Timestamp
        {
            get
            {
                return timeStamp;
            }
        }

        public MeasType Type
        {
            get
            {
                return type;
            }
        }

        
        private readonly UInt16 index;
        private readonly string sValue;
        private readonly string flags;
        private readonly DateTime timeStamp;
        private readonly MeasType type;       
    }
}
