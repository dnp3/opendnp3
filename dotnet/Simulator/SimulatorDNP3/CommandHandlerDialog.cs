using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public partial class CommandHandlerDialog : Form
    {
        public CommandHandlerDialog(string label)
        {
            InitializeComponent();

            this.Text = label;

            this.comboBoxCode.DataSource = Enum.GetValues(typeof(CommandStatus));            
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public UInt16 SelectedIndex
        {
            get
            {
                return Decimal.ToUInt16(this.numericUpDown1.Value);
            }
        }

        public CommandStatus SelectedCode
        {
            get
            {
                return (CommandStatus)comboBoxCode.SelectedItem;
            }
        }
    }
}
