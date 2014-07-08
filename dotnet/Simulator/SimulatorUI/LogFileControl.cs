using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.Commons;

namespace Automatak.Simulator.UI
{
    public partial class LogFileControl : UserControl
    {
        LogToFile log = null;

        public LogFileControl()
        {
            InitializeComponent();
        }

        void CheckState()
        {
            if (log != null)
            {
                this.buttonPlay.Enabled = true;
                this.buttonPause.Enabled = true;
            }
            else
            {
                this.buttonPlay.Enabled = false;
                this.buttonPause.Enabled = false;
            }
        }
    }
}
