using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Automatak.DNP3.Simulator
{
    public partial class LogWindow : UserControl
    {
        public LogWindow()
        {
            InitializeComponent();
            this.UpdateStatus();
        }

        public void AddRows(IEnumerable<string> lines)
        {
            this.logControl.AddRows(lines);
            this.UpdateStatus();
        }        

        public Font LogFont
        {
            get { return logControl.Font; }
            set
            {
                logControl.Font = value;
            }
        }

        void UpdateStatus()
        { 
            var format = "Capacity: {0,5} / {1,-5} View: ";

            this.toolStripStatusLabel.Text = String.Format(
                format, 
                logControl.NumRows,
                logControl.MaxRows);            
        }

        private void buttonPause_Click(object sender, EventArgs e)
        {
            this.buttonPause.Enabled = false;
            this.buttonPlay.Enabled = true;
            this.logControl.Pause();            
        }

        private void buttonPlay_Click(object sender, EventArgs e)
        {
            this.buttonPause.Enabled = true;
            this.buttonPlay.Enabled = false;
            this.logControl.Resume();           
        }

        private void buttonClipboard_Click(object sender, EventArgs e)
        {
            var rows = this.logControl.GetViewportRows();            
            Clipboard.SetText(String.Join(Environment.NewLine, rows.ToArray()));
        }
    }
}
