using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public interface IMasterControl
    {
        ICommandAcceptor CommandAcceptor
        {
            set;
        }
    }
    

    public partial class StackDisplayControl : UserControl
    {
        public StackDisplayControl()
        {
            InitializeComponent();
        }

        public IMasterControl AddMaster(string name, IMeasurementSource source)
        {
            var display = new MasterDataDisplay(source);
            display.Dock = DockStyle.Fill;
            TabPage page = new TabPage(name);
            this.tabControlStacks.TabPages.Add(page);
            page.Controls.Add(display);
            return display;
        }
    }

    
}
