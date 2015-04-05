using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class ChannelDialog : Form
    {
        public ChannelDialog()
        {
            InitializeComponent();            
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
                toolStripStatusLabel1.Text = "Unable to create channel";
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
                    return GetTCPServerFunctor(min, max);
                case(2):
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

            var flags = logLevelControl1.Filters.Flags;
            var ss = new SerialSettings(name, baud, dataBits, stopBits, parity, flow);
            return (IDNP3Manager manager) => manager.AddSerial(this.textBoxID.Text, flags, min, max, ss);
        }

        private Func<IDNP3Manager, IChannel> GetTCPClientFunctor(TimeSpan min, TimeSpan max)
        {
            var flags = logLevelControl1.Filters.Flags;
            return (IDNP3Manager manager) => manager.AddTCPClient(this.textBoxID.Text, flags, min, max, textBoxHost.Text, Decimal.ToUInt16(numericUpDownPort.Value));
        }

        private Func<IDNP3Manager, IChannel> GetTCPServerFunctor(TimeSpan min, TimeSpan max)
        {
            var flags = logLevelControl1.Filters.Flags;
            return (IDNP3Manager manager) => manager.AddTCPServer(this.textBoxID.Text, flags, min, max, textBoxServerHost.Text, Decimal.ToUInt16(numericUpDownServerPort.Value));
        } 

        public Func<IDNP3Manager, IChannel> ChannelAction
        {
            get
            {
                return create;
            }
        }

        public String SelectedAlias
        {
            get
            {
                return textBoxID.Text;
            }
        }        

        private Func<IDNP3Manager, IChannel> create = null;     
    }
}
