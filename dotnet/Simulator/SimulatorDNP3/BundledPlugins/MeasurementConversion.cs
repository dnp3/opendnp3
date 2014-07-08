using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    static class MeasurementConversion
    {
        public static Measurement ToMeasurement(this Binary meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.Binary, index, QualityInfo.binary);
        }

        public static Measurement ToMeasurement(this DoubleBitBinary meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.DoubleBitBinary, index, QualityInfo.doubleBinary);
        }

        public static Measurement ToMeasurement(this Counter meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.Counter, index, QualityInfo.counter);
        }

        public static Measurement ToMeasurement(this FrozenCounter meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.FrozenCounter, index, QualityInfo.counter);
        }

        public static Measurement ToMeasurement(this Analog meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.Analog, index, QualityInfo.analog);
        }

        public static Measurement ToMeasurement(this BinaryOutputStatus meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.BinaryOutputStatus, index, QualityInfo.binaryOutputStatus);
        }

        public static Measurement ToMeasurement(this AnalogOutputStatus meas, UInt16 index)
        {
            return new Measurement(meas.Value.ToString(), meas, MeasType.AnalogOutputStatus, index, QualityInfo.analog);
        }

        public static Measurement ToMeasurement(this OctetString meas, UInt16 index)
        {
            return new Measurement(meas.AsString(), meas, MeasType.OctetString, index, QualityInfo.octetString);
        }       
    }
}
