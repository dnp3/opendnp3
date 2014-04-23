using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace DNP3TestHarness
{
    public partial class MasterDialog : Form
    {
        public MasterDialog()
        {
            InitializeComponent();            
        }

        private void buttonADD_Click(object sender, EventArgs e)
        {            
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public MasterStackConfig Configuration
        {
            get
            {
                var config = new MasterStackConfig();
                config.link.localAddr = Decimal.ToUInt16(numericUpDownSource.Value);
                config.link.remoteAddr = Decimal.ToUInt16(numericUpDownDest.Value);
                return config;
            }
        }
    }
}
