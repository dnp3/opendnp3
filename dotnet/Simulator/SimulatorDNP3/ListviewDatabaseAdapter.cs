using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class ListviewDatabaseAdapter : IDatabase
    {
        readonly ListBox listBox;

        ListviewDatabaseAdapter(ListBox listBox)
        {
            this.listBox = listBox;
        }

        public static void Process(IEnumerable<Action<IDatabase>> actions, ListBox listBox)
        {
            IDatabase adapter = new ListviewDatabaseAdapter(listBox);
            
            adapter.Start();
            foreach (var action in actions)
            {
                action.Invoke(adapter);
            }
            adapter.End();
        }

        void IDatabase.Start()
        {
            listBox.SuspendLayout();
        }

        bool Add(Measurement meas, string label)
        {
            var text = string.Format("{0} ({1}) - {2} - {3}", label, meas.Index, meas.Value, meas.ShortFlags);        
            listBox.Items.Add(text);
            return true;
        }

        bool IDatabase.Update(Binary update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "Binary");
        }

        bool IDatabase.Update(DoubleBitBinary update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "DoubleBitBinary");
        }

        bool IDatabase.Update(Analog update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "Analog");
        }

        bool IDatabase.Update(Counter update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "Counter");
        }

        bool IDatabase.Update(FrozenCounter update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "FrozenCounter");
        }

        bool IDatabase.Update(BinaryOutputStatus update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "BinaryOutputStatus");
        }

        bool IDatabase.Update(AnalogOutputStatus update, ushort index, EventMode mode)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "AnalogOutputStatus");
        }

        bool IDatabase.Update(TimeAndInterval update, ushort index)
        {
            return this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "TimeAndInterval");
        }

        void IDatabase.End()
        {
            listBox.ResumeLayout();
        }        

        bool IDatabase.Modify(Func<Binary, Binary> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<DoubleBitBinary, DoubleBitBinary> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<Analog, Analog> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<Counter, Counter> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<FrozenCounter, FrozenCounter> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<BinaryOutputStatus, BinaryOutputStatus> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<AnalogOutputStatus, AnalogOutputStatus> update, ushort index, EventMode mode)
        {
            return false;
        }

        bool IDatabase.Modify(Func<TimeAndInterval, TimeAndInterval> update, ushort index)
        {
            return false;
        }
    }
}
