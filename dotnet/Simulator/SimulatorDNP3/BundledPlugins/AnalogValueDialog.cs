using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public partial class AnalogValueDialog : Form
    {
        readonly bool isAnalog;
        readonly IEnumerable<ushort> indices;

        public AnalogValueDialog(bool isAnalog, IEnumerable<ushort> indices)
        {
            InitializeComponent();

            this.isAnalog = isAnalog;
            this.indices = indices;

            this.qualitySelector.QualityInfo = QualityInfo.analog;

            this.numericUpDown1.Minimum = Decimal.MinValue;
            this.numericUpDown1.Maximum = Decimal.MaxValue;
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
                var value = Decimal.ToDouble(numericUpDown1.Value);
                var timestamp = DateTime.Now;
                var quality = qualitySelector.Quality;
                

                if (isAnalog)
                {
                    return indices.Select(i => MeasActions.GetAnalogAction(value, quality, timestamp, i));                    
                }
                else
                {
                    return indices.Select(i => MeasActions.GetAnalogOutputStatusAction(value, quality, timestamp, i));
                }
            }
        }        
    }
}
