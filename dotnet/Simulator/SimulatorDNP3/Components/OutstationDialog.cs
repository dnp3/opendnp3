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

        OutstationParams OutstationParameters
        {
            get
            {
                OutstationParams ret = new OutstationParams();
                ret.allowUnsolicited = this.checkBoxEnableUnsol.Checked;
                ret.maxControlsPerRequest = Decimal.ToByte(this.numericUpDownMaxControls.Value);
                ret.maxTxFragSize = Decimal.ToUInt16(this.numericUpDownMaxTxFrag.Value);
                ret.selectTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownSelectTimeout.Value));
                ret.unsolicitedConfirmTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownUnsolConfirmTimeout.Value));
                ret.solicitedConfirmTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownSolConfirmTimeout.Value));
                ret.unsolicitedRetryPeriod = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownUnsolicitedRetry.Value));
                return ret;
            }
        }

        public OutstationStackConfig Configuration
        {
            get
            {
                var config = new OutstationStackConfig();
                config.link = this.linkConfigControl.Configuration;
                config.outstation.config = this.OutstationParameters;     
                config.defaultStaticResponses = this.staticResponseTypeControl1.Configuration;
                config.defaultEventResponses = this.eventResponseTypeControl1.Configuration;
                return config;
            }
        }
    }
}
