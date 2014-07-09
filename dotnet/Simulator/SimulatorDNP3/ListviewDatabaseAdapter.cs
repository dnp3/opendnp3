using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;

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
            var text = string.Format("{0} Event ({1}) - {2} - {3}", label, meas.Index, meas.Value, meas.ShortFlags);        
            listBox.Items.Add(text);
        }


        void IDatabase.Update(Binary update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "Binary");
        }

        void IDatabase.Update(DoubleBitBinary update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "DoubleBitBinary");
        }

        void IDatabase.Update(Analog update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "Analog");
        }

        void IDatabase.Update(Counter update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "Counter");
        }

        void IDatabase.Update(FrozenCounter update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "FrozenCounter");
        }

        void IDatabase.Update(BinaryOutputStatus update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "BinaryOutputStatus");
        }

        void IDatabase.Update(AnalogOutputStatus update, ushort index)
        {
            this.Add(update.ToMeasurement(index), "AnalogOutputStatus");
        }

        void IDatabase.End()
        {
            listBox.ResumeLayout();
        }        
    }
}
