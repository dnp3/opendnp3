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
            }
        }

        public LinkConfig Configuration
        {
            get
            {
                var config = new LinkConfig(isMaster, false);
                config.localAddr = Decimal.ToUInt16(this.numericUpDownSource.Value);
                config.remoteAddr = Decimal.ToUInt16(this.numericUpDownDest.Value);
                config.useConfirms = this.checkBoxConfirmed.Checked;
                return config;
            }
        }

        private void Configure()
        {
            this.numericUpDownSource.Value = LinkConfig.GetDefaultSourceAddress(isMaster);
            this.numericUpDownDest.Value = LinkConfig.GetDefaultDestinationAddress(isMaster);
            this.checkBoxConfirmed.Checked = false;
        }

        private bool isMaster = false;
    }
}
