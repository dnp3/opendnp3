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
            this.valueAsString = sValue;
            this.flags = GetQualityAsString(meas.Quality, info);
            this.timeStamp = meas.IsTimestampValid ? meas.Timestamp : DateTime.Now;                        
            this.timeStampAssumed = !meas.IsTimestampValid;
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
                return valueAsString;
            }
        }

        public string Flags
        {
            get
            {
                return flags;
            }
        }

        public string Timestamp
        {
            get
            {
                var time = timeStamp.ToString("d") + timeStamp.ToString(" HH:mm:ss.fff");
                if (timeStampAssumed)
                {
                    return time + " (assumed)";
                }
                else
                {
                    return time;
                }
            }
        }        

        public MeasType Type
        {
            get
            {
                return type;
            }
        }

        string GetQualityAsString(byte quality, IQualityBitInfo info)
        {
            var start = "0x" + quality.ToString("X02") + " - ";

            if (QualityInfo.CountOf(quality) > 2)
            {
                return start + String.Join(", ", QualityInfo.GetShortNames(quality, info));
            }
            else
            {
                return start + String.Join(", ", QualityInfo.GetLongNames(quality, info));
            }            
        }

        
        readonly UInt16 index;
        readonly string valueAsString;
        readonly string flags;
        readonly DateTime timeStamp;
        readonly bool timeStampAssumed;
        readonly MeasType type;       
    }
}
