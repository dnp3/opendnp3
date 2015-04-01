using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    public partial class CROBControl : UserControl
    {
        public CROBControl()
        {
            InitializeComponent();

            this.comboBoxCode.DataSource = Enum.GetValues(typeof(ControlCode));
        }

        public ControlRelayOutputBlock ControlValue
        {
            get
            {
                var code = (ControlCode)this.comboBoxCode.SelectedItem;
                return new ControlRelayOutputBlock(code, 1, 1000, 1000);
            }
        }
    }
}
