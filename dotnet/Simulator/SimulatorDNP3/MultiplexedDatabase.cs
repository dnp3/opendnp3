using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class MultiplexedDatabase : IDatabase
    {
        IEnumerable<IDatabase> databases;

        public MultiplexedDatabase(params IDatabase[] databases)
        {
            this.databases = new List<IDatabase>(databases);
        }       

        bool IDatabase.Update(Binary update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(DoubleBitBinary update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(Analog update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(Counter update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(FrozenCounter update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(BinaryOutputStatus update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(AnalogOutputStatus update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index, mode));
        }

        bool IDatabase.Update(TimeAndInterval update, ushort index)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Update(update, index));
        }

        void IDatabase.End()
        {
            foreach (var db in databases)
            {
                db.End();
            }
        }


        bool IDatabase.Modify(Func<Binary, Binary> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<DoubleBitBinary, DoubleBitBinary> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<Analog, Analog> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<Counter, Counter> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<FrozenCounter, FrozenCounter> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<BinaryOutputStatus, BinaryOutputStatus> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<AnalogOutputStatus, AnalogOutputStatus> update, ushort index, EventMode mode)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index, mode));
        }

        bool IDatabase.Modify(Func<TimeAndInterval, TimeAndInterval> update, ushort index)
        {
            return databases.Aggregate(true, (sum, db) => sum && db.Modify(update, index));
        }
    }
}
