using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.Commons;
using Automatak.Simulator.DNP3.API;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class EventBufferConfigControl : UserControl
    {       
        const UInt16 defaultValue = 100;
       
        static IEnumerable<MeasType> types = new MeasType[] { 
           MeasType.Binary,
           MeasType.DoubleBitBinary,
           MeasType.Counter,
           MeasType.FrozenCounter,
           MeasType.Analog,
           MeasType.BinaryOutputStatus,
           MeasType.AnalogOutputStatus
        };

        public EventBufferConfigControl()
        {
            InitializeComponent();
            
            foreach (var t in types)
            {                
                listView1.Items.Add(CreateItem(t.ToString()));
            }

            this.CheckState();
        }

        public EventBufferConfig Configuration
        {
            get
            {
                var config = new EventBufferConfig();
                int i = 0;
                foreach (ListViewItem item in listView1.Items)
                {
                    var value = UInt32.Parse(item.SubItems[1].Text);
                    SetValue(config, i, value);
                    ++i;            
                }

                return config;
            }
        }

        void SetValue(EventBufferConfig config, int index, UInt32 value)
        { 
            switch(index)
            {
                case(0):
                    config.maxBinaryEvents = value;
                    break;
                case (1):
                    config.maxDoubleBinaryEvents = value;
                    break;
                case (2):
                    config.maxCounterEvents = value;
                    break;
                case (3):
                    config.maxFrozenCounterEvents = value;
                    break;
                case (4):
                    config.maxAnalogEvents = value;
                    break;
                case (5):
                    config.maxBinaryOutputStatusEvents = value;
                    break;
                case (6):
                    config.maxAnalogOutputStatusEvents = value;
                    break;
            }            
        }

        void CheckState()
        {
            if (this.listView1.SelectedItems.Count > 0)
            {
                this.numericUpDown1.Enabled = true;
            }
            else
            {
                this.numericUpDown1.Enabled = false;
            }
        }

        ListViewItem CreateItem(string value)
        { 
            var strings = new String[] { value, defaultValue.ToString() };
            return new ListViewItem(strings);
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.CheckState();
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            this.CheckState();
            foreach (ListViewItem item in listView1.SelectedItems)
            {
                item.SubItems[1].Text = Decimal.ToUInt16(numericUpDown1.Value).ToString();
            }
        }
    }
}
