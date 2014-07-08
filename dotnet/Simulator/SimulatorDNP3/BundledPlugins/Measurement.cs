using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
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
        public Measurement(string sValue, MeasurementBase meas, MeasType type, UInt16 index, IQualityBitInfo info)
        {
            this.sValue = sValue;
            this.flags = "0x" + meas.Quality.ToString("X02") + " - " + String.Join(", ", QualityInfo.GetLongNames(meas.Quality, info));
            this.timeStamp = meas.Timestamp;            
            this.type = type;
            this.index = index;
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
