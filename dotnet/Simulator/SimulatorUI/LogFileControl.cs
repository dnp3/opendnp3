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

        public LogFileControl()
        {
            InitializeComponent();

            var tooltip1 = new System.Windows.Forms.ToolTip();
            tooltip1.SetToolTip(this.buttonBrowse, "Browse for a log file");

            var tooltip2 = new System.Windows.Forms.ToolTip();
            tooltip2.SetToolTip(this.buttonStop, "Stop logging to file");
            

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
                this.buttonStop.Enabled = false;
                this.buttonBrowse.Enabled = false;
            }
            else
            {
                if (enabled)
                {                    
                    this.buttonStop.Enabled = true;
                    this.buttonBrowse.Enabled = false;
                }
                else
                {                                        
                    this.buttonStop.Enabled = false;
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
                    this.enabled = true;
                    log.LogFilePath = dialog.FileName;
                    log.Enabled = true;
                    this.CheckState();
                }                
            }
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            this.enabled = false;
            log.Enabled = false;
            this.CheckState();
        }
       
    }
}
