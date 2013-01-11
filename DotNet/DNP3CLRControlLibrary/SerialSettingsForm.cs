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
    public partial class SerialSettingsForm : Form
    {
        public delegate void OnSerialPortAdded(SerialSettings settings);

        public event OnSerialPortAdded SerialPortAdded;

        private String[] baud = { "1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200" };
        private String[] dataBits = { "5", "6", "7", "8", "9" };
        private String[] parity = { "even", "odd", "none" };
        private String[] stopBits = { "1", "2" };
        private String[] flowControl = { "hardware", "xon/xoff", "none" };

        public SerialSettingsForm()
        {
            InitializeComponent();
            this.comboBoxPort.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());            
            this.comboBoxBaud.Items.AddRange(baud);
            this.comboBoxDataBits.Items.AddRange(dataBits);
            this.comboBoxParity.Items.AddRange(parity);
            this.comboBoxStopBits.Items.AddRange(stopBits);
            this.comboBoxFlowControl.Items.AddRange(flowControl);

            //set the default display values
            if (this.comboBoxPort.Items.Count > 0) this.comboBoxPort.Text = this.comboBoxPort.Items[0].ToString();
            this.comboBoxBaud.SelectedIndex = 3;
            this.comboBoxDataBits.SelectedIndex = 3;
            this.comboBoxParity.SelectedIndex = 2;
            this.comboBoxStopBits.SelectedIndex = 0;
            this.comboBoxFlowControl.SelectedIndex = 2;

        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            
            try
            {
                String port = this.comboBoxPort.Text;
                int baud = int.Parse(this.comboBoxBaud.Text);
                int dataBits = int.Parse(this.comboBoxDataBits.Text);
                Parity parity = this.GetParity();
                int stopBits = int.Parse(this.comboBoxStopBits.Text);
                FlowControl flowControl = this.GetFlowControl();
                SerialSettings ss = new SerialSettings(port, baud, dataBits, stopBits, parity, flowControl);
                if (this.SerialPortAdded != null) SerialPortAdded(ss);
                this.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }                        
        }

        private FlowControl GetFlowControl()
        {
            switch (this.comboBoxFlowControl.SelectedIndex)
            { 
                case(0):
                    return FlowControl.HARDWARE;
                case(1):
                    return FlowControl.XONXOFF;
                case(2):
                    return FlowControl.NONE;
                default:
                    return FlowControl.NONE;
            }
        }

        private Parity GetParity()
        {
            switch(this.comboBoxParity.SelectedIndex)
            {
                case(0):
                    return Parity.EVEN;
                case(1):
                    return Parity.ODD;
                case(2):
                    return Parity.NONE;
                default:
                    return Parity.NONE;
            }
        }

    }
}
