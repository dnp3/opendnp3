using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace DotNetTestSet
{
    public partial class LogControl : UserControl
    {
        public LogControl()
        {
            InitializeComponent();
        }

        public void AddLogEntry(LogEntry le)
        {
            var items = new String[] { le.filterLevel.ToString(), le.time.ToLongTimeString(), le.message };
            this.flickerFreeListView1.Items.Insert(0, new ListViewItem(items));            
        }
    }
}
