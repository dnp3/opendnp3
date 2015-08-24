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

        public ChangeSet SelectedChanges
        {
            get
            {
                var timestamp = DateTime.Now;
                var changes = new ChangeSet();

                if (isBinary)
                {
                    indices.Each(i => changes.Update(new Binary(checkBoxValue.Checked, qualitySelector.Quality, timestamp), i));                    
                }
                else
                {
                    indices.Each(i => changes.Update(new BinaryOutputStatus(checkBoxValue.Checked, qualitySelector.Quality, timestamp), i));
                }

                return changes;
            }
        }        
    }
}
