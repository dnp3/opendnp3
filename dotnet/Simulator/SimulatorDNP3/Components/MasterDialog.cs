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
    public partial class MasterDialog : Form
    {
        public MasterDialog()
        {
            InitializeComponent();

            this.linkConfigControl.IsMaster = true;
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
                config.link = this.linkConfigControl.Configuration;
                config.master = this.masterConfigControl.Configuration;
                return config;
            }
        }
    }
}
