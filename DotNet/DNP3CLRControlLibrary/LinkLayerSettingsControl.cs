using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public partial class LinkLayerSettingsControl : UserControl
    {
        private bool isMaster = true;

        public LinkLayerSettingsControl()
        {
            InitializeComponent();            
        }

        public LinkConfig Config
        {
            get
            {
                UInt16 local = Decimal.ToUInt16(this.numericUpDownLocalAddress.Value);
                UInt16 remote = Decimal.ToUInt16(this.numericUpDownRemoteAddress.Value);
                UInt64 timeout = Decimal.ToUInt64(this.numericUpDownTimeout.Value);
                UInt32 retries = Decimal.ToUInt32(this.numericUpDownRetries.Value);
                return new LinkConfig(isMaster, checkBoxUseConfirms.Checked, retries, local, remote, timeout);
            }
        }

        public bool IsMaster
        {
            get
            {
                return isMaster;
            }
            set
            {
                isMaster = value;
                this.numericUpDownLocalAddress.Value = isMaster ? 1 : 1024;
                this.numericUpDownRemoteAddress.Value = isMaster ? 1024 : 1;
            }

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }
    }
}
