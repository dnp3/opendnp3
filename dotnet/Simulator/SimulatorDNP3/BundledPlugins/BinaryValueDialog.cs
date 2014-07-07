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
    public partial class BinaryValueDialog : Form
    {
        public BinaryValueDialog()
        {
            InitializeComponent();            
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public bool SelectedValue
        {
            get
            {
                return this.checkBoxValue.Checked;
            }
        }

        public byte SelectedQuality
        {
            get
            {
                return 0x01;
            }
        }
    }
}
