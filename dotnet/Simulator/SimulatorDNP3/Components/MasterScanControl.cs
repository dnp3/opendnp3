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

namespace Automatak.Simulator.DNP3.Components
{
    public partial class MasterScanControl : UserControl
    {
        IMaster master = null;

        readonly ICollection<IMasterScan> scans = new List<IMasterScan>();

        public MasterScanControl()
        {
            InitializeComponent();

            this.ContextMenuStrip = this.contextMenuStrip1;
        }

        public IMaster Master
        {
            set
            {
                this.master = value;
            }
        }

        private void addScanToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var dialog = new ScanDialog(master))
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    var scans = dialog.ConfiguredScans;
                }
            }
        }
       
    }
}
