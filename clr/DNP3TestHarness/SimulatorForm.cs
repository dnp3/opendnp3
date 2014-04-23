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

        public SimulatorForm(IDNP3Manager manager, bool suppressSplash)
        {
            this.manager = manager;
            this.suppressSplash = suppressSplash;

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
            this.listBoxLog.Items.Add(GetLogString(entry));
            if (listBoxLog.Items.Count > 500)
            {
                listBoxLog.Items.RemoveAt(0);
            }
            listBoxLog.SelectedIndex = listBoxLog.Items.Count - 1;
            listBoxLog.SelectedIndex = -1;
        }

        private string GetLogString(LogEntry entry)
        {
            return entry.time.ToString("HH:mm:ss.fff") + " - " + LogFilters.GetFilterString(entry.filter.Flags) + " - " + entry.loggerName + " - " + entry.message;
        }

        private IDNP3Manager manager;
        private bool suppressSplash;        

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
            
            if (!suppressSplash)
            {
                this.BeginInvoke(new Action(() => ShowAboutBox()));
            }
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
    }
}
