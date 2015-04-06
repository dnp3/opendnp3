using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{
    public partial class CounterValueDialog : Form
    {
        readonly bool isCounter;
        readonly IEnumerable<ushort> indices;

        public CounterValueDialog(bool isCounter, IEnumerable<ushort> indices)
        {
            InitializeComponent();

            this.isCounter = isCounter;
            this.indices = indices;

            this.qualitySelector.QualityInfo = QualityInfo.counter;

            this.numericUpDown1.Minimum = 0;
            this.numericUpDown1.Maximum = UInt32.MaxValue;
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public ChangeSet SelectedChanges
        {        
            get
            {
                var value = Decimal.ToUInt32(numericUpDown1.Value);
                var quality = qualitySelector.Quality;
                var timestamp = DateTime.Now;
                var changes = new ChangeSet();            
                if (isCounter)
                {
                    indices.Each(i => changes.Update(new Counter(value, quality, timestamp), i));
                }
                else
                {
                    indices.Each(i => changes.Update(new FrozenCounter(value, quality, timestamp), i));
                }
                return changes;
            }
        }        
    }
}
