using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class OutstationDialog : Form
    {
        public OutstationDialog()
        {
            InitializeComponent();

            this.linkConfigControl.IsMaster = false;
        }

        private void buttonADD_Click(object sender, EventArgs e)
        {            
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public String SelectedAlias
        {
            get
            {
                return textBoxID.Text;
            }
        }

        public OutstationStackConfig Configuration
        {
            get
            {
                var config = new OutstationStackConfig();
                config.link = this.linkConfigControl.Configuration;
                config.defaultStaticResponses = this.staticResponseTypeControl1.Configuration;
                config.defaultEventResponses = this.eventResponseTypeControl1.Configuration;
                return config;
            }
        }
    }
}
