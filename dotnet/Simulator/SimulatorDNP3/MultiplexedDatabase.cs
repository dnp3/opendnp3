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

        bool IDatabase.Update(Binary update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(DoubleBitBinary update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(Analog update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(Counter update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(FrozenCounter update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(BinaryOutputStatus update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(AnalogOutputStatus update, ushort index, bool forceEvent)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index, forceEvent);
            }
            return ret;
        }

        bool IDatabase.Update(TimeAndInterval update, ushort index)
        {
            bool ret = true;
            foreach (var db in databases)
            {
                ret &= db.Update(update, index);
            }
            return ret;
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
