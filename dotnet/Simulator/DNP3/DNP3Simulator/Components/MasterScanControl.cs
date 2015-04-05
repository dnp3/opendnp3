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

        readonly ICollection<ScanInfo> scans = new List<ScanInfo>();

        public MasterScanControl()
        {
            InitializeComponent();            
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
                    foreach (var scan in dialog.ConfiguredScans)
                    {
                        scans.Add(scan);
                    }

                    this.RefreshScanList();
                }
            }
        }

        private void RefreshScanList()
        {
            this.listViewScans.SuspendLayout();

            try
            {
                this.listViewScans.Items.Clear();
                foreach (var info in scans)
                {
                    var item = new ListViewItem(new string[] { info.description, info.period.ToString() });
                    item.ToolTipText = info.details;
                    item.Tag = info.scan;
                    this.listViewScans.Items.Add(item);
                }
            }
            finally
            {
                this.listViewScans.ResumeLayout();
            }            
        }

        private void listViewScans_MouseDown(object sender, MouseEventArgs e)
        {
            
            if (e.Button != System.Windows.Forms.MouseButtons.Right)
            {
                return;
            }

            ListViewItem choice = null;
                      
            foreach (ListViewItem item in listViewScans.Items)
            {
                if (item.Bounds.Contains(new Point(e.X, e.Y)))
                {
                        choice = item;
                }
            }


            if (choice == null)
            {
                this.contextMenuStripAdd.Show(listViewScans, e.X, e.Y);
            }
            else
            {
                this.GetScanMenu(((IMasterScan)choice.Tag)).Show(listViewScans, e.X, e.Y);
            }            
        }

        ContextMenuStrip GetScanMenu(IMasterScan scan)
        {
            var menu = new ContextMenuStrip();
            var demand = new ToolStripMenuItem("Demand");
            demand.Click += (object sender, EventArgs e) => {
                scan.Demand();
            };
            menu.Items.Add(demand);
            return menu;
        }
       
    }
}
