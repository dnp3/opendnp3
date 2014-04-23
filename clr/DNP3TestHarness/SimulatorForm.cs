using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;

namespace DNP3TestHarness
{
    public partial class SimulatorForm : Form
    {
        class LogAdapter : ILogHandler
        {
            public LogAdapter(Action<LogEntry> action)
            {
                this.action = action;
            }

            void ILogHandler.Log(LogEntry entry)
            {
                action(entry);
            }

            private Action<LogEntry> action;
        }

        public SimulatorForm(IDNP3Manager manager)
        {
            this.manager = manager;
            this.logFollowMode = true;

            InitializeComponent();

            this.WindowState = FormWindowState.Maximized;
            
            manager.AddLogHandler(new LogAdapter(OnLogEntry));                                          
        }

        private void OnLogEntry(LogEntry entry)
        {            
            this.BeginInvoke(new Action(() => this.AddLogEntry(entry)));
        }

        private void AddLogEntry(LogEntry entry)
        {           
            WriteLogMessage(entry.first, GetLogString(entry));                       
        }

        private void WriteLogMessage(bool first, String msg)
        {
            if (first)
            {
                this.listBoxLog.Items.Add("");
            }

            this.listBoxLog.Items.Add(msg);

            if (listBoxLog.Items.Count > 500)
            {
                listBoxLog.Items.RemoveAt(0);
            }

            if (logFollowMode)
            {
                listBoxLog.TopIndex = listBoxLog.Items.Count - 1;
            }
        }

        private string GetLogString(LogEntry entry)
        {            
            if (entry.first)
            {                
                string preamble = String.Format("{0,-16}{1,-10}{2,-20}", entry.time.ToString("HH:mm:ss.fff"), LogFilters.GetFilterString(entry.filter.Flags), entry.loggerName);                
                return String.Format("  {0}{1}", preamble, entry.message);
            }
            else
            {
                string format = "  {0}{1}";
                return String.Format(format, new String(' ', 46), entry.message);
            }            
        }

        private void WriteSystemLog(String message)
        { 
            var preamble = String.Format("{0,-16}{1,-10}{2,-20}", DateTime.Now.ToString("HH:mm:ss.fff"), LogFilters.GetFilterString(LogFilters.EVENT), "simulator");
            var output = String.Format("  {0}{1}", preamble, message);
            WriteLogMessage(true, output);
        }

        private IDNP3Manager manager;
        private bool logFollowMode;

        private void ShowAboutBox()
        {
            using (About about = new About())
            {
                about.ShowDialog();
            }            
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowAboutBox();  
        }

        private void TestHarnessForm_Load(object sender, EventArgs e)
        {                                    
            WriteSystemLog("Add a channel or load a configuration to begin");
        }

        private void clearWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.listBoxLog.Items.Clear();
        }

        private void addToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (ChannelDialog dialog = new ChannelDialog(Enumerable.Empty<string>()))
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    var channel = dialog.ChannelAction.Invoke(manager);
                    this.commTreeView.AddChannel(dialog.ChannelId, channel);
                }
            }
        }

        private void pausedToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (sender.GetType() == typeof(ToolStripMenuItem))
            {
                var item = (ToolStripMenuItem) sender;
                if (item.Checked)
                {
                    item.Checked = false;
                    this.logFollowMode = true;
                }
                else
                {
                    item.Checked = true;
                    this.logFollowMode = false;
                }
            }
        }
    }
}
