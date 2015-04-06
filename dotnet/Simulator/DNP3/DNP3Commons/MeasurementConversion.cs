using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Commons
{
    public static class MeasurementConversion
    {
        public static Measurement ToMeasurement(this Binary meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.Binary, index, QualityInfo.binary);
        }

        public static Measurement ToMeasurement(this DoubleBitBinary meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.DoubleBitBinary, index, QualityInfo.doubleBinary);
        }

        public static Measurement ToMeasurement(this Counter meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.Counter, index, QualityInfo.counter);
        }

        public static Measurement ToMeasurement(this FrozenCounter meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.FrozenCounter, index, QualityInfo.counter);
        }

        public static Measurement ToMeasurement(this Analog meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.Analog, index, QualityInfo.analog);
        }

        public static Measurement ToMeasurement(this BinaryOutputStatus meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.BinaryOutputStatus, index, QualityInfo.binaryOutputStatus);
        }

        public static Measurement ToMeasurement(this AnalogOutputStatus meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.Value.ToString(), meas, tsmode, MeasType.AnalogOutputStatus, index, QualityInfo.analog);
        }

        public static Measurement ToMeasurement(this TimeAndInterval meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.ToString(), tsmode, MeasType.TimeAndInterval, index, QualityInfo.octetString);
        }

        public static Measurement ToMeasurement(this OctetString meas, UInt16 index, TimestampMode tsmode)
        {
            return new Measurement(meas.AsString(), tsmode, MeasType.OctetString, index, QualityInfo.octetString);
        }       
    }
}
