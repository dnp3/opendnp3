using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using System.Threading;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.Commons;

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

    class MeasurementCache: ISOEHandler, IMeasurementCache, IDatabase
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
        readonly MeasurementCollection timeAndIntervals = new MeasurementCollection();

        public MeasurementCache(DatabaseTemplate template)
        {
            var info = new HeaderInfo(GroupVariation.UNKNOWN, QualifierCode.UNDEFINED, TimestampMode.SYNCHRONIZED, 0);

            ISOEHandler handler = this;

            /*
            handler.OnReceiveHeader(info, template.binaries.SelectWithIndex((m, i) => new IndexedValue<Binary>(new Binary(false), i)));
            handler.OnReceiveHeader(info, template.doubleBinaries.SelectWithIndex((m, i) => new IndexedValue<DoubleBitBinary>(new DoubleBitBinary(m.quality), i)));
            handler.OnReceiveHeader(info, template.counters.SelectWithIndex((m, i) => new IndexedValue<Counter>(new Counter(m.quality), i)));
            handler.OnReceiveHeader(info, template.frozenCounters.SelectWithIndex((m, i) => new IndexedValue<FrozenCounter>(new FrozenCounter(m.quality), i)));
            handler.OnReceiveHeader(info, template.analogs.SelectWithIndex((m, i) => new IndexedValue<Analog>(new Analog(m.quality), i)));
            handler.OnReceiveHeader(info, template.binaryOutputStatii.SelectWithIndex((m, i) => new IndexedValue<BinaryOutputStatus>(new BinaryOutputStatus(m.quality), i)));
            handler.OnReceiveHeader(info, template.analogOutputStatii.SelectWithIndex((m, i) => new IndexedValue<AnalogOutputStatus>(new AnalogOutputStatus(m.quality), i)));
            */
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
                case(MeasType.TimeAndInterval):
                    return timeAndIntervals;
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
        

        void IDatabase.Update(Binary update, ushort index, EventMode mode)
        {
            binaries.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(DoubleBitBinary update, ushort index, EventMode mode)
        {
            doubleBinaries.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(Analog update, ushort index, EventMode mode)
        {
            analogs.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(Counter update, ushort index, EventMode mode)
        {
            counters.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(FrozenCounter update, ushort index, EventMode mode)
        {
            frozenCounters.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(BinaryOutputStatus update, ushort index, EventMode mode)
        {
            binaryOutputStatii.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(AnalogOutputStatus update, ushort index, EventMode mode)
        {
            analogOutputStatii.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }

        void IDatabase.Update(TimeAndInterval update, ushort index)
        {
            timeAndIntervals.Update(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED));            
        }


        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<Binary>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            binaries.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<DoubleBitBinary>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            doubleBinaries.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<Analog>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            analogs.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<Counter>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            counters.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<FrozenCounter>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            frozenCounters.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<BinaryOutputStatus>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            binaryOutputStatii.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<AnalogOutputStatus>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            analogOutputStatii.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<OctetString>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            octetStrings.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<TimeAndInterval>> values)
        {
            var converted = values.Select(m => m.Value.ToMeasurement(m.Index, info.tsmode));
            octetStrings.Update(converted);
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<BinaryCommandEvent>> values)
        {
            // TODO
        }

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<AnalogCommandEvent>> values)
        {
            // TODO
        }      

        void ISOEHandler.Process(HeaderInfo info, IEnumerable<IndexedValue<SecurityStat>> values)
        {
           
        }
    }
}
