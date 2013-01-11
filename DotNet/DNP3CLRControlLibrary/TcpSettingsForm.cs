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
    public partial class TcpSettingsForm : Form
    {       
        public delegate void AddTcp(TcpSettings settings);
        public event AddTcp OnTcpAdded;               

        public TcpSettingsForm(String banner, String defaultPortName)
        {
            InitializeComponent();
            this.Text = banner;
            this.textBoxName.Text = defaultPortName;
            this.FormClosed += new FormClosedEventHandler(TcpSettingsForm_FormClosed);
        }

        void TcpSettingsForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if(DialogResult == DialogResult.OK && OnTcpAdded != null) 
            {
                String name = this.textBoxName.Text;
                String address = this.textBoxAddress.Text;
                UInt16 port = Decimal.ToUInt16(this.numericUpDownPort.Value);
                FilterLevel level = this.filterLevelComboBox1.SelectedLevel;
                UInt64 timeout = Decimal.ToUInt64(this.numericUpDownTimeout.Value);
                OnTcpAdded(new TcpSettings(name, address, port, level, timeout));            
            }
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        
    }
}
