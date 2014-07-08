using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
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

        public IEnumerable<Action<IDatabase>> SelectedActions
        {
            get
            {
                var value = Decimal.ToUInt32(numericUpDown1.Value);
                var timestamp = DateTime.Now;
                var quality = qualitySelector.Quality;
                

                if (isCounter)
                {
                    return indices.Select(i => MeasActions.GetCounterAction(value, quality, timestamp, i));                    
                }
                else
                {
                    return indices.Select(i => MeasActions.GetFrozenCounterAction(value, quality, timestamp, i));
                }
            }
        }        
    }
}
