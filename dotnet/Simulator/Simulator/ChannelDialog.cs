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
            this.comboBoxParity.DataSource = Enum.GetValues(typeof(Parity));
            this.comboBoxStopBits.DataSource = Enum.GetValues(typeof(StopBits));
            this.comboBoxFlowControl.DataSource = Enum.GetValues(typeof(FlowControl));

            this.comboBoxParity.SelectedItem = Parity.NONE;
            this.comboBoxStopBits.SelectedItem = StopBits.ONE;
            this.comboBoxFlowControl.SelectedItem = FlowControl.NONE;
        }

        private void buttonADD_Click(object sender, EventArgs e)
        {            
            create = GetCreateFunctorMaybeNull();
            if (create == null)
            {
                statusStrip.Text = "Unable to create channel";
            }
            else
            { 
                this.DialogResult = DialogResult.OK;
                this.Close();
            }            
        }

        private Func<IDNP3Manager, IChannel> GetCreateFunctorMaybeNull()
        {
            var min = TimeSpan.FromMilliseconds(Decimal.ToDouble(numericUpDownMinRetryMS.Value));
            var max = TimeSpan.FromMilliseconds(Decimal.ToDouble(numericUpDownMaxRetryMS.Value));

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
            var baud = Decimal.ToInt32(this.numericUpDownBaud.Value);
            
            var dataBits = Decimal.ToInt32(this.numericUpDownDataBits.Value);
            var parity = (Parity)comboBoxParity.SelectedValue;
            var flow = (FlowControl) comboBoxFlowControl.SelectedValue;
            var stopBits = (StopBits) comboBoxStopBits.SelectedValue;

            var ss = new SerialSettings(name, baud, dataBits, stopBits, parity, flow);
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
