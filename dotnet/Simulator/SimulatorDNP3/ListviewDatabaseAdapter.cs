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

        void Add(Measurement meas, string label)
        {
            var text = string.Format("{0} ({1}) - {2} - {3}", label, meas.Index, meas.Value, meas.ShortFlags);        
            listBox.Items.Add(text);
        }


        void IDatabase.Update(Binary update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "Binary");
        }

        void IDatabase.Update(DoubleBitBinary update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "DoubleBitBinary");
        }

        void IDatabase.Update(Analog update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "Analog");
        }

        void IDatabase.Update(Counter update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "Counter");
        }

        void IDatabase.Update(FrozenCounter update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "FrozenCounter");
        }

        void IDatabase.Update(BinaryOutputStatus update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "BinaryOutputStatus");
        }

        void IDatabase.Update(AnalogOutputStatus update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "AnalogOutputStatus");
        }

        void IDatabase.Update(TimeAndInterval update, ushort index)
        {
            this.Add(update.ToMeasurement(index, TimestampMode.SYNCHRONIZED), "TimeAndInterval");
        }

        void IDatabase.End()
        {
            listBox.ResumeLayout();
        }        
    }
}
