using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;

namespace Automatak.Simulator
{
    partial class MakeNoteDialog : Form
    {
        public MakeNoteDialog()
        {
            InitializeComponent();
        }

        public string[] SelectedLines
        {
            get
            {
                return textBox1.Lines;
            }
        }

        private void buttonAccept_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }                       
    }
}
