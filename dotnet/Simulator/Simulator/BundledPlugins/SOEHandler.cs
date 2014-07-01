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

        void Add<T>(IEnumerable<T> values, Func<T, Measurement> convert)
        {
            var converted = values.Select(convert);
            foreach (var v in converted)
            {
                measurements.Add(v);
            }   
        }
        
        void ISOEHandler.End()
        {
            if (measurements.Any() && NewMeasurements != null)
            {               
               NewMeasurements(measurements);                
            }           
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<Binary>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Binary, m.Index));           
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.DoubleBitBinary, m.Index));
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<Analog>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Analog, m.Index));
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<Counter>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Counter, m.Index));
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.FrozenCounter, m.Index));
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.BinaryOutputStatus, m.Index));
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.AnalogOutputStatus, m.Index));
        }

        void ISOEHandler.LoadStatic(IEnumerable<IndexedValue<OctetString>> values)
        {
            Add(values, m => new Measurement(m.Value.AsString(), m.Value, MeasType.OctetString, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<Binary>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Binary, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.DoubleBitBinary, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<Analog>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Analog, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<Counter>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Counter, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.FrozenCounter, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            Add(values, m => new Measurement(m.Value.ToString(), m.Value, MeasType.BinaryOutputStatus, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            Add(values, m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.AnalogOutputStatus, m.Index));
        }

        void ISOEHandler.LoadEvent(IEnumerable<IndexedValue<OctetString>> values)
        {
            Add(values, m => new Measurement(m.Value.AsString(), m.Value, MeasType.OctetString, m.Index));
        }
    }
}
