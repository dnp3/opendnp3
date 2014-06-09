using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    public partial class ChannelDialog : Form
    {
        public ChannelDialog(IEnumerable<string> takenNames)
        {
            InitializeComponent();
            this.takenNames = takenNames;
            this.comboBoxSerialDeviceName.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
        }

        private void buttonADD_Click(object sender, EventArgs e)
        {            
            create = GetCreateFunctorMaybeNull();
            if (create != null)
            {
                this.DialogResult = DialogResult.OK;
                this.Close();
            }            
        }

        private Func<IDNP3Manager, IChannel> GetCreateFunctorMaybeNull()
        {
            var min = TimeSpan.FromMilliseconds(Decimal.ToDouble(numericUpDownMinRetryMS.Value));
            var max = TimeSpan.FromMilliseconds(Decimal.ToDouble(numericUpDownMaxRetry.Value));

            switch (tabControlChannelType.SelectedIndex)
            { 
                case(0):
                    return GetTCPClientFunctor(min, max);
                case(1):
                    return GetSerialFunctor(min, max);
                default:
                    return null;
            }
        }

        private Func<IDNP3Manager, IChannel> GetSerialFunctor(TimeSpan min, TimeSpan max)
        {
            var name = this.comboBoxSerialDeviceName.Text;
            var ss = new SerialSettings(name, 9600, 8, 1, Parity.NONE, FlowControl.NONE);
            return (IDNP3Manager manager) => manager.AddSerial(this.textBoxID.Text, GetFilters(), min, max, ss);
        }

        private Func<IDNP3Manager, IChannel> GetTCPClientFunctor(TimeSpan min, TimeSpan max)
        {
            return (IDNP3Manager manager) => manager.AddTCPClient(this.textBoxID.Text, GetFilters(), min, max, textBoxHost.Text, Decimal.ToUInt16(numericUpDownPort.Value));
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

            if (checkBoxAppHeader.Checked)
            {
                filters |= (LogFilters.APP_HEADER_RX | LogFilters.APP_HEADER_TX);
            }

            if (checkBoxAppObject.Checked)
            {
                filters |= (LogFilters.APP_OBJECT_RX | LogFilters.APP_OBJECT_TX);
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
