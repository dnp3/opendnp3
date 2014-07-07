using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using System.Threading;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    static class Helpers
    { 
        public static IEnumerable<B> SelectWithIndex<A,B>(this IEnumerable<A> seq, Func<A, ushort, B> selector)
        {
            ushort i = 0;
            
            return seq.Select(x => {
                var num = i;
                ++i;
                return selector(x, num);
            });
        }
    }

    class MeasurementCache: ISOEHandler, IMeasurementCache
    {
        readonly Object mutex = new Object();

        readonly MeasurementCollection binaries = new MeasurementCollection();
        readonly MeasurementCollection doubleBinaries = new MeasurementCollection();
        readonly MeasurementCollection counters = new MeasurementCollection();
        readonly MeasurementCollection frozenCounters = new MeasurementCollection();
        readonly MeasurementCollection analogs = new MeasurementCollection();
        readonly MeasurementCollection binaryOutputStatii = new MeasurementCollection();
        readonly MeasurementCollection analogOutputStatii = new MeasurementCollection();
        readonly MeasurementCollection octetStrings = new MeasurementCollection();

        public MeasurementCache(DatabaseTemplate template)
        {
            LoadStatic(template.binaries.SelectWithIndex((m, i) => new IndexedValue<Binary>(new Binary(m.quality), i)));
            LoadStatic(template.doubleBinaries.SelectWithIndex((m, i) => new IndexedValue<DoubleBitBinary>(new DoubleBitBinary(m.quality), i)));
            LoadStatic(template.counters.SelectWithIndex((m, i) => new IndexedValue<Counter>(new Counter(m.quality), i)));
            LoadStatic(template.frozenCounters.SelectWithIndex((m, i) => new IndexedValue<FrozenCounter>(new FrozenCounter(m.quality), i)));
            LoadStatic(template.analogs.SelectWithIndex((m, i) => new IndexedValue<Analog>(new Analog(m.quality), i)));
            LoadStatic(template.binaryOutputStatii.SelectWithIndex((m, i) => new IndexedValue<BinaryOutputStatus>(new BinaryOutputStatus(m.quality), i)));
            LoadStatic(template.analogOutputStatii.SelectWithIndex((m, i) => new IndexedValue<AnalogOutputStatus>(new AnalogOutputStatus(m.quality), i)));            
        }

        public MeasurementCache()
        {

        }

        void ISOEHandler.Start()
        {
            Monitor.Enter(mutex);
        }      
        
        void ISOEHandler.End()
        {
            Monitor.Exit(mutex);          
        }

        public void LoadStatic(IEnumerable<IndexedValue<Binary>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Binary, m.Index));
            binaries.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.DoubleBitBinary, m.Index));
            doubleBinaries.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<Analog>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Analog, m.Index));
            analogs.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<Counter>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.Counter, m.Index));
            counters.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.FrozenCounter, m.Index));
            frozenCounters.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.BinaryOutputStatus, m.Index));
            binaryOutputStatii.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.Value.ToString(), m.Value, MeasType.AnalogOutputStatus, m.Index));
            analogOutputStatii.Update(converted);
        }

        public void LoadStatic(IEnumerable<IndexedValue<OctetString>> values)
        {
            var converted = values.Select(m => new Measurement(m.Value.AsString(), m.Value, MeasType.OctetString, m.Index));
            octetStrings.Update(converted);
        }

        public void LoadEvent(IEnumerable<IndexedValue<Binary>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<Analog>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<Counter>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            LoadStatic(values);
        }

        public void LoadEvent(IEnumerable<IndexedValue<OctetString>> values)
        {
            LoadStatic(values);
        }

        MeasurementCollection GetCollectionMaybeNull(MeasType type)
        {
            switch (type)
            {
                case (MeasType.Binary):
                    return binaries;
                case (MeasType.DoubleBitBinary):
                    return doubleBinaries;
                case (MeasType.Counter):
                    return counters;
                case (MeasType.FrozenCounter):
                    return frozenCounters;
                case (MeasType.Analog):
                    return analogs;
                case (MeasType.BinaryOutputStatus):
                    return binaryOutputStatii;
                case (MeasType.AnalogOutputStatus):
                    return analogOutputStatii;
                case (MeasType.OctetString):
                    return octetStrings;
                default:
                    return null;
            }
        }

        MeasurementCollection IMeasurementCache.Binaries
        {
            get { return binaries; }
        }

        MeasurementCollection IMeasurementCache.DoubleBinaries
        {
            get { return doubleBinaries; }
        }

        MeasurementCollection IMeasurementCache.Counters
        {
            get { return counters; }
        }

        MeasurementCollection IMeasurementCache.FrozenCounters
        {
            get { return frozenCounters; }
        }

        MeasurementCollection IMeasurementCache.Analogs
        {
            get { return analogs; }
        }

        MeasurementCollection IMeasurementCache.BinaryOutputStatii
        {
            get { return binaryOutputStatii; }
        }

        MeasurementCollection IMeasurementCache.AnalogOutputStatii
        {
            get { return analogOutputStatii; }
        }

        public MeasurementCollection OctetStrings
        {
            get { return octetStrings; }
        }

        public MeasurementCollection GetCollection(MeasType type)
        {
            switch (type)
            { 
                case(MeasType.Binary):
                    return binaries;
                case (MeasType.DoubleBitBinary):
                    return doubleBinaries;
                case (MeasType.Counter):
                    return counters;
                case (MeasType.FrozenCounter):
                    return frozenCounters;
                case (MeasType.Analog):
                    return analogs;
                case (MeasType.BinaryOutputStatus):
                    return binaryOutputStatii;
                case (MeasType.AnalogOutputStatus):
                    return analogOutputStatii;
                case(MeasType.OctetString):
                    return octetStrings;
                default:
                    return null;
            }
        }

        
    }
}
