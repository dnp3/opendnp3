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

namespace Automatak.DNP3.Simulator
{
    public partial class SimulatorForm : Form
    {
        class LogQueue : ILogHandler
        {
            public delegate void OnNewEntries();

            public event OnNewEntries NewEntries;           

            public ICollection<LogEntry> Flush()
            {
                lock (mutex)
                {
                    flushed = true;
                    var list = new List<LogEntry>(entries);
                    entries.Clear();
                    return list;
                }
            }

            void ILogHandler.Log(LogEntry entry)
            {
                lock(mutex)
                {
                    entries.Enqueue(entry);
                    if(flushed)
                    {
                        flushed = false;
                        if (NewEntries != null)
                        {
                            NewEntries();
                        }
                    }
                }
            }

            private Object mutex = new Object();
            private bool flushed = true;
            private Queue<LogEntry> entries = new Queue<LogEntry>();
        }

        public SimulatorForm(IDNP3Manager manager, IEnumerable<IMasterPluginFactory> masterPlugins)
        {
            this.manager = manager;
            this.masterPlugins = masterPlugins;
            this.logFollowMode = true;

            InitializeComponent();

            this.WindowState = FormWindowState.Maximized;
            
            manager.AddLogHandler(logQueue);                                          
        }      

        private void AddLogEntries(ICollection<LogEntry> entries)
        {
            var messages = entries.Select(le => GetLogMessage(le));
            var lines = messages.SelectMany(msg => msg.Lines);
        }

        class LogMessage
        {
            public LogMessage(bool first, String message)
            {
                this.first = first;
                this.message = message;
            }

            public IEnumerable<String> Lines
            {
                get
                {
                    if (first)
                    {
                        yield return "";
                    }
                    yield return message;
                }
            }

            private readonly bool first;
            private readonly string message;
        }

        private LogMessage GetLogMessage(LogEntry entry)
        {            
            if (entry.first)
            {                
                string preamble = String.Format("{0,-16}{1,-10}{2,-20}", entry.time.ToString("HH:mm:ss.fff"), LogFilters.GetFilterString(entry.filter.Flags), entry.loggerName);
                return new LogMessage(true, String.Format("  {0}{1}", preamble, entry.message));
            }
            else
            {
                string format = "  {0}{1}";
                return new LogMessage(false, String.Format(format, new String(' ', 46), entry.message));
            }            
        }

        private LogMessage GetSystemLogMessage(String message)
        { 
            var preamble = String.Format("{0,-16}{1,-10}{2,-20}", DateTime.Now.ToString("HH:mm:ss.fff"), LogFilters.GetFilterString(LogFilters.EVENT), "simulator");
            return new LogMessage(true, String.Format("  {0}{1}", preamble, message));            
        }

        private IDNP3Manager manager;
        private LogQueue logQueue = new LogQueue();
        IEnumerable<IMasterPluginFactory> masterPlugins;
        private bool logFollowMode;

        void ShowAboutBox()
        {
            using (About about = new About())
            {
                about.ShowDialog();
            }            
        }

        void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowAboutBox();  
        }

        void WriteLinesToListBox(IEnumerable<String> lines)
        {
            listBoxLog.Items.AddRange(lines.ToArray());

            while(listBoxLog.Items.Count > 500)
            {
                listBoxLog.Items.RemoveAt(0);
            }

            if (this.logFollowMode)
            {
                listBoxLog.TopIndex = listBoxLog.Items.Count - 1;
            }
        }

        void TestHarnessForm_Load(object sender, EventArgs e)
        {
            logQueue.NewEntries += logQueue_NewEntries;
            var lines = GetSystemLogMessage("Add a channel or load a configuration to begin").Lines;
            WriteLinesToListBox(lines);
        }

        void logQueue_NewEntries()
        {
            this.BeginInvoke(new Action(() => FlushLogQueue()));
        }

        void FlushLogQueue()
        {
            var lines = logQueue.Flush().Select(le => GetLogMessage(le)).SelectMany(msg => msg.Lines);
            WriteLinesToListBox(lines);
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
                    this.commTreeView.AddChannel(dialog.ChannelId, channel, masterPlugins);
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
