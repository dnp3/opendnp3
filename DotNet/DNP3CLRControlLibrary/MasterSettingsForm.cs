using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace TestSetControlLibrary
{    
    public partial class MasterSettingsForm : Form
    {
        public delegate void AddMaster(String name, FilterLevel level, MasterStackConfig config);
        public event AddMaster OnAddMaster;

        public MasterSettingsForm()
        {
            InitializeComponent();
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void MasterSettingsForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (DialogResult == DialogResult.OK && OnAddMaster != null)
            {                
                MasterStackConfig config = new MasterStackConfig();
                config.link = this.linkLayerSettingsControl1.Config;
                OnAddMaster(this.textBoxName.Text, FilterLevel.LEV_INTERPRET, config);
            }
        }
    }
}
