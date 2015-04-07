using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Automatak.Simulator.DNP3.RelayOutstationPlugin
{
    public partial class OutstationForm : Form
    {
        public OutstationForm()
        {
            InitializeComponent();

        }

        private void transparentButtonTrip_Click(object sender, EventArgs e)
        {
            
        }

        private void transparentButtonClose_Click(object sender, EventArgs e)
        {
            
        }

        private void OutstationForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }
    }
}
