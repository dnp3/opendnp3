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
        readonly ListView listView;

        ListviewDatabaseAdapter(ListView listView)
        {
            this.listView = listView;
        }

        public static void Process(IEnumerable<Action<IDatabase>> actions, ListView listView)
        {
            IDatabase adapter = new ListviewDatabaseAdapter(listView);
            
            adapter.Start();
            foreach (var action in actions)
            {
                action.Invoke(adapter);
            }
            adapter.End();
        }

        void IDatabase.Start()
        {
            listView.SuspendLayout();
        }

        void Add(Measurement meas, string label)
        {
            string[] text = { meas.Index.ToString(), meas.Value, meas.Flags, meas.Timestamp, label };
            var item = new ListViewItem(text);
            listView.Items.Add(item);
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
            listView.ResumeLayout();
        }        
    }
}
