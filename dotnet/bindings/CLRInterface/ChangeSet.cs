using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public class ChangeSet : IDatabase
    {
        private IList<Action<IDatabase>> updates = new List<Action<IDatabase>>();

        public void Apply(IDatabase database)
        {
            foreach(var action in updates) 
            {
                action(database);
            }
        }      

        public void Update(Binary update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(DoubleBitBinary update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(Analog update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(Counter update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(FrozenCounter update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(BinaryOutputStatus update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(AnalogOutputStatus update, ushort index, EventMode mode = EventMode.Detect)
        {
            updates.Add((IDatabase db) => db.Update(update, index, mode));
        }

        public void Update(TimeAndInterval update, ushort index)
        {
            updates.Add((IDatabase db) => db.Update(update, index));
        }
    }
}
