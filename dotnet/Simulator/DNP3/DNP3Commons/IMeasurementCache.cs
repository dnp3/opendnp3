using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.Simulator.DNP3.Commons
{
    public interface IMeasurementCache
    {
        MeasurementCollection GetCollection(MeasType type);

        MeasurementCollection Binaries
        {
            get;
        }

        MeasurementCollection DoubleBinaries
        {
            get;
        }

        MeasurementCollection Counters
        {
            get;
        }

        MeasurementCollection FrozenCounters
        {
            get;
        }

        MeasurementCollection Analogs
        {
            get;
        }

        MeasurementCollection BinaryOutputStatii
        {
            get;
        }

        MeasurementCollection AnalogOutputStatii
        {
            get;
        }

        MeasurementCollection OctetStrings
        {
            get;
        }
    }
}
