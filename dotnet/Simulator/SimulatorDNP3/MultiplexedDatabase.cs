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

        void IDatabase.Update(Binary update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(DoubleBitBinary update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(Analog update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(Counter update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(FrozenCounter update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(BinaryOutputStatus update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(AnalogOutputStatus update, ushort index, EventMode mode)
        {
            foreach (var db in databases) db.Update(update, index, mode);
        }

        void IDatabase.Update(TimeAndInterval update, ushort index)
        {
            foreach (var db in databases) db.Update(update, index);
        }       
    }
}
