using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{       
    class SOEHandler: ISOEHandler
    {
        public delegate void OnMeasurements(IEnumerable<Measurement> measurements);
        public event OnMeasurements NewMeasurements;

        IList<Measurement> measurements = null;
        
        void ISOEHandler.Start()
        {
            measurements = new List<Measurement>();            
        }

        void ISOEHandler.End()
        {
            if (measurements.Any() && NewMeasurements != null)
            {               
               NewMeasurements(measurements);                
            }           
        }

        void ISOEHandler.LoadStatic(Binary meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.Binary, index));
        }

        void ISOEHandler.LoadStatic(DoubleBitBinary meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.DoubleBitBinary, index));
        }

        void ISOEHandler.LoadStatic(Analog meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.Analog, index));
        }

        void ISOEHandler.LoadStatic(Counter meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.Counter, index));
        }

        void ISOEHandler.LoadStatic(FrozenCounter meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.FrozenCounter, index));
        }

        void ISOEHandler.LoadStatic(BinaryOutputStatus meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.BinaryOutputStatus, index));
        }

        void ISOEHandler.LoadStatic(AnalogOutputStatus meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.AnalogOutputStatus, index));
        }

        void ISOEHandler.LoadStatic(OctetString meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.AsString(), meas, MeasType.OctetString, index));
        }

        void ISOEHandler.LoadEvent(Binary meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.Binary, index));
        }

        void ISOEHandler.LoadEvent(DoubleBitBinary meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.DoubleBitBinary, index));
        }

        void ISOEHandler.LoadEvent(Analog meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.Analog, index));
        }

        void ISOEHandler.LoadEvent(Counter meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.Counter, index));
        }

        void ISOEHandler.LoadEvent(FrozenCounter meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.FrozenCounter, index));
        }

        void ISOEHandler.LoadEvent(BinaryOutputStatus meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.BinaryOutputStatus, index));
        }

        void ISOEHandler.LoadEvent(AnalogOutputStatus meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.Value.ToString(), meas, MeasType.AnalogOutputStatus, index));
        }

        void ISOEHandler.LoadEvent(OctetString meas, ushort index)
        {
            this.measurements.Add(new Measurement(meas.AsString(), meas, MeasType.OctetString, index));
        }
    }
}
