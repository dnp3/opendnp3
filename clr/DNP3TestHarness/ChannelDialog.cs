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
    public partial class ChannelDialog : Form
    {
        public ChannelDialog(IEnumerable<string> takenNames)
        {
            InitializeComponent();
            this.takenNames = takenNames;
        }

        private void buttonADD_Click(object sender, EventArgs e)
        {
            var min = TimeSpan.FromMilliseconds(Decimal.ToDouble(numericUpDownMinRetryMS.Value));
            var max = TimeSpan.FromMilliseconds(Decimal.ToDouble(numericUpDownMaxRetry.Value));
            create = (IDNP3Manager manager) => manager.AddTCPClient(this.textBoxID.Text, GetFilters(), min, max, textBoxHost.Text, Decimal.ToUInt16(numericUpDownPort.Value));
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private uint GetFilters()
        {
            uint filters = LogFilters.EVENT;
            if (checkBoxError.Checked) filters |= LogFilters.ERROR;
            if (checkBoxWarn.Checked) filters |= LogFilters.WARNING;
            if (checkBoxInfo.Checked) filters |= LogFilters.INFO;
            if (checkBoxDebug.Checked) filters |= LogFilters.DEBUG;

            if (checkBoxLink.Checked)
            {
                filters |= (LogFilters.LINK_RX | LogFilters.LINK_TX);
            }

            if (checkBoxLinkRaw.Checked)
            {
                filters |= (LogFilters.LINK_RX_HEX | LogFilters.LINK_TX_HEX);
            }

            if (checkBoxTransport.Checked)
            {
                filters |= (LogFilters.TRANSPORT_RX | LogFilters.TRANSPORT_TX);
            }

            return filters;
        }

        public Func<IDNP3Manager, IChannel> ChannelAction
        {
            get
            {
                return create;
            }
        }

        public String ChannelId
        {
            get
            {
                return this.textBoxID.Text;
            }
        }


        private Func<IDNP3Manager, IChannel> create = null;
        private IEnumerable<string> takenNames;
    }
}
