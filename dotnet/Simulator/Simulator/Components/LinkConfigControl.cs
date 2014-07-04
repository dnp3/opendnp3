using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator.Components
{
    public partial class LinkConfigControl : UserControl
    {
        public LinkConfigControl()
        {
            InitializeComponent();
        }

        public bool IsMaster 
        {
            set
            {
                isMaster = value;
                this.Configure();
            }
        }

        public LinkConfig Configuration
        {
            get
            {
                var config = new LinkConfig(isMaster, this.checkBoxConfirmed.Checked);
                config.localAddr = Decimal.ToUInt16(this.numericUpDownSource.Value);
                config.remoteAddr = Decimal.ToUInt16(this.numericUpDownDest.Value);
                config.numRetry = Decimal.ToUInt32(this.numericUpDownRetries.Value);
                config.timeoutMs = Decimal.ToUInt32(this.numericUpDownTimeout.Value);
                return config;
            }
        }

        private void SetState()
        {
            this.groupBoxConfirmed.Enabled = this.checkBoxConfirmed.Checked;            
        }

        private void Configure()
        {
            this.numericUpDownSource.Value = LinkConfig.GetDefaultSourceAddress(isMaster);
            this.numericUpDownDest.Value = LinkConfig.GetDefaultDestinationAddress(isMaster);
            this.numericUpDownTimeout.Value = LinkConfig.DefaultTimeoutMillisconds;
            this.numericUpDownRetries.Value = LinkConfig.DefaultNumRetries;
            this.checkBoxConfirmed.Checked = false;

            this.SetState();
        }

        private bool isMaster = false;

        private void checkBoxConfirmed_CheckedChanged(object sender, EventArgs e)
        {
            this.SetState();
        }     
    }
}
