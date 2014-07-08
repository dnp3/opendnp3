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
        bool enabled = false;
        bool hasPath = false;

        public LogFileControl()
        {
            InitializeComponent();

            this.CheckState();
        }

        public LogToFile FileLogger
        {
            set
            {
                log = value;
                this.CheckState();
            }
        }

        void CheckState()
        {
            if (log == null)
            {
                this.buttonPlay.Enabled = false;
                this.buttonPause.Enabled = false;
                this.buttonBrowse.Enabled = false;
            }
            else
            {
                if (enabled)
                {
                    this.buttonPlay.Enabled = false;
                    this.buttonPause.Enabled = true;
                    this.buttonBrowse.Enabled = false;
                }
                else
                {                    
                    this.buttonPlay.Enabled = hasPath;
                    this.buttonPause.Enabled = false;
                    this.buttonBrowse.Enabled = true;
                }
            }             
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            using (var dialog = new OpenFileDialog())
            {
                dialog.Title = "Open or create a log file";
                dialog.Multiselect = false;
                dialog.Filter = "Text files (*.txt)|*.txt";
                if(dialog.ShowDialog() == DialogResult.OK)
                {
                    this.hasPath = true;
                    log.LogFilePath = dialog.FileName;
                    this.CheckState();
                }                
            }
        }

        private void buttonPlay_Click(object sender, EventArgs e)
        {
            this.log.Enabled = true;
            this.enabled = true;
            this.CheckState();
        }

        private void buttonPause_Click(object sender, EventArgs e)
        {
            this.log.Enabled = false;
            this.enabled = false;
            this.CheckState();
        }
    }
}
