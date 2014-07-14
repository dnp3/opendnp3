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
    public partial class BinaryValueDialog : Form
    {
        readonly bool isBinary;
        readonly IEnumerable<ushort> indices;

        public BinaryValueDialog(bool isBinary, IEnumerable<ushort> indices)
        {
            InitializeComponent();

            this.isBinary = isBinary;
            this.indices = indices;

            this.qualitySelector.QualityInfo = isBinary ? QualityInfo.binary : QualityInfo.binaryOutputStatus;
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
                var timestamp = DateTime.Now;

                if (isBinary)
                {
                    return indices.Select(i => MeasActions.GetBinaryAction(checkBoxValue.Checked, qualitySelector.Quality, timestamp, i));                    
                }
                else
                {
                    return indices.Select(i => MeasActions.GetBinaryOutputStatusAction(checkBoxValue.Checked, qualitySelector.Quality, timestamp, i));
                }
            }
        }        
    }
}
