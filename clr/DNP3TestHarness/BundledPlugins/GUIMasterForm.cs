using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    public partial class GUIMasterForm : Form
    {
        private SOEHandler handler = new SOEHandler();

        public ISOEHandler SequenceOfEvents
        {
            get { return handler; }
            
        }        

        public GUIMasterForm()
        {
            InitializeComponent();            
        }

        void handler_Updated()
        {            
                     
        }

        private void GUIMasterForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }       

        private void GUIMasterForm_Load(object sender, EventArgs e)
        {
            this.dataGridView1.DataSource = handler.BinarySource;
        }
    }
}
