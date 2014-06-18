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
        public SimulatorForm(IDNP3Manager manager, IEnumerable<IMasterPluginFactory> masterPlugins)
        {
            this.manager = manager;
            this.masterPlugins = masterPlugins;            

            InitializeComponent();           
            
            manager.AddLogHandler(logQueue);                                          
        }
        

        private IDNP3Manager manager;
        private LogProcessor logQueue = new LogProcessor();
        IEnumerable<IMasterPluginFactory> masterPlugins;        

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
       
        void TestHarnessForm_Load(object sender, EventArgs e)
        {
            logQueue.LogSystemMessage("Add a channel or load a configuration to begin");            
        }     

        void FlushLogQueue()
        {
            var lines = logQueue.Flush();
            this.logWindow1.AddRows(lines);            
        }

        private void clearWindowToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //logControl.Clear();
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

        private void logFlushTimer_Tick(object sender, EventArgs e)
        {
            FlushLogQueue(); 
        }             
    }
}
