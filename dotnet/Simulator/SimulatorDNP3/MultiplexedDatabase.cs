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

        void IDatabase.Start()
        {
            foreach (var db in databases)
            {
                db.Start();
            }
        }

        void IDatabase.Update(Binary update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(DoubleBitBinary update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(Analog update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(Counter update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(FrozenCounter update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(BinaryOutputStatus update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(AnalogOutputStatus update, ushort index, bool forceEvent)
        {
            foreach (var db in databases)
            {
                db.Update(update, index, forceEvent);
            }
        }

        void IDatabase.Update(TimeAndInterval update, ushort index)
        {
            foreach (var db in databases)
            {
                db.Update(update, index);
            }
        }

        void IDatabase.End()
        {
            foreach (var db in databases)
            {
                db.End();
            }
        }        
    }
}
