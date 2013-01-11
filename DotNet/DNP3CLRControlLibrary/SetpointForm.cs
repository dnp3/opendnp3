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
    public partial class SetpointForm : Form
    {
        ICommandAcceptor cmdAcceptor;

        public SetpointForm(ICommandAcceptor cmdAcceptor)
        {
            InitializeComponent();
            this.cmdAcceptor = cmdAcceptor;
        }

        private Setpoint GetSetpoint()
        {
            switch (this.comboBoxType.SelectedIndex)
            {
                case (0): 
                    return new Setpoint(Decimal.ToUInt16(this.numericUpDownValue.Value));
                case (1):
                    return new Setpoint(Decimal.ToUInt32(this.numericUpDownValue.Value));
                case (2):
                    return new Setpoint(Decimal.ToSingle(this.numericUpDownValue.Value));
                default:
                    return new Setpoint(Decimal.ToDouble(this.numericUpDownValue.Value));
            }
        }

        private void buttonIssue_Click(object sender, EventArgs e)
        {
            this.groupBoxParameters.Enabled = false;
            this.textBoxStatus.Text = "Executing...";
            var future = cmdAcceptor.AcceptCommand(GetSetpoint(), Decimal.ToUInt32(this.numericUpDownIndex.Value));
            future.Listen(OnResult);
        }

        private void OnResult(CommandStatus status)
        {
            Action invoke = () => OnMarshalledResult(status);            
            this.BeginInvoke(invoke);
        }

        private void OnMarshalledResult(CommandStatus status)
        {
            this.groupBoxParameters.Enabled = true;
            this.textBoxStatus.Text = status.ToString();
        }
    }
}
