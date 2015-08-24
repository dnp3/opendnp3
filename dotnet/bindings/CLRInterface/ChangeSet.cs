using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public interface IChangeSet
    {
        void Apply(IDatabase database);        
    }

    public class ChangeSet : IDatabase, IChangeSet
    {
        private IList<Action<IDatabase>> updates = new List<Action<IDatabase>>();

        void IChangeSet.Apply(IDatabase database)
        {
            foreach(var action in updates) 
            {
                action(database);
            }
        }        

        public void Clear()
        {
            updates.Clear();
        }

        public bool IsEmpty()
        {
            return updates.Count == 0;
        }

        public int Count
        {
            get
            {
                return updates.Count;
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
