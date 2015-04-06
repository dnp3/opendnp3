using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.API;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
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

        public IEnumerable<Action<ChangeSet>> SelectedActions
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

        public ChangeSet SelectedChanges
        {
            get
            {
                var value = Decimal.ToDouble(numericUpDown1.Value);
                var quality = qualitySelector.Quality;
                var timestamp = DateTime.Now;
                var changes = new ChangeSet();            
                if (isAnalog)
                {
                    indices.Each(i => changes.Update(new Analog(value, quality, timestamp), i));
                }
                else
                {
                    indices.Each(i => changes.Update(new AnalogOutputStatus(value, quality, timestamp), i));
                }
                return changes;
            }
        } 
    }
}
