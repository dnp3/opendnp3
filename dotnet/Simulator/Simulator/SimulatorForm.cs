using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.UI;
using Automatak.Simulator.Commons;
using Automatak.Simulator.API;

namespace Automatak.Simulator
{
    public partial class SimulatorForm : Form
    {
        readonly IEnumerable<ISimulatorPluginFactory> plugins;
        readonly bool splashOnLoad;
        readonly LogToFile fileLogger;
        readonly ILog log;


        public SimulatorForm(IEnumerable<ISimulatorPluginFactory> plugins, bool splashOnLoad)
        {                 
            InitializeComponent();    
            
            this.plugins = plugins;
            this.splashOnLoad = splashOnLoad;
            this.fileLogger = new LogToFile();
            this.log = new LogMultiplexer(this.logWindow1, fileLogger);

            this.logFileControl.FileLogger = fileLogger;

            var toolTipUserNote = new ToolTip();
            toolTipUserNote.SetToolTip(this.buttonMakeNote, "Make a user-defined note in the log");
        }
           
        void ShowAboutBox()
        {
            using (var about = new AboutBox())
            {                
                about.ShowDialog();
            }            
        }

        void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ShowAboutBox();  
        }

        private void BindNode(ISimulatorNode simNode, TreeNode node, TreeNodeCollection parent)
        {
            node.Text = simNode.DisplayName;
            node.Tag = simNode;

            var menu = new ContextMenu();

            foreach (var nodeAction in simNode.Actions)
            {
                var action = new MenuItem(nodeAction.DisplayName);
                action.Click += new EventHandler(
                    delegate(Object o, EventArgs a)
                    {
                        nodeAction.Invoke();
                    }
                );
                menu.MenuItems.Add(action);
            }

            if (simNode.Actions.Any())
            {
                menu.MenuItems.Add("-");
            }
            
            foreach (var factory in simNode.Children)
            {
                var action = new MenuItem(factory.DisplayName);
                action.Click += new EventHandler(
                    delegate(Object o, EventArgs a)
                    {
                        var callbacks = new TreeNodeCallbacks(this);
                        var child = factory.Create(callbacks);
                        if (child != null)
                        {
                            this.BindNode(child, callbacks.node, node.Nodes);
                        }
                    }
                );
                menu.MenuItems.Add(action);
            }

            if (simNode.Children.Any())
            {
                menu.MenuItems.Add("-");
            }

            var item = new MenuItem("Remove");
            item.Click += new EventHandler(
                delegate(Object o, EventArgs a)
                {
                    ShutdownFrom(node);
                    parent.Remove(node);
                }
            );
           
            menu.MenuItems.Add(item);
            node.ContextMenu = menu;
            parent.Add(node);
        }

        private static void ShutdownFrom(TreeNode node)
        {
            var simNode = node.Tag as ISimulatorNode;
            foreach(TreeNode subnode in node.Nodes)
            {
                ShutdownFrom(subnode);
            }
            simNode.Remove();
        }
        
        private void SimulatorForm_Load(object sender, EventArgs e)
        {                        
            foreach (var factory in plugins)
            {                
                var instance = factory.Create(this.log);                
                var item = new ToolStripMenuItem(instance.RootDisplayName);
                item.Image = instance.PluginImage;
                this.addToolStripMenuItem.DropDownItems.Add(item);
                var page = new TabPage(instance.UniqueId);                
                var treeView = new TreeView();
                page.Tag = treeView;
                treeView.Dock = DockStyle.Fill;
                treeView.ImageList = instance.NodeImageList;
                page.Controls.Add(treeView);
                this.tabControlPlugins.TabPages.Add(page);
                
                item.Click += new EventHandler(
                    delegate(Object o, EventArgs a)
                    {                        
                        var callbacks = new TreeNodeCallbacks(this);
                        var node = instance.Create(callbacks);
                        if (node != null)
                        {
                            BindNode(node, callbacks.node, treeView.Nodes);
                        }
                    }
                );

                this.log.LogFull(DisplayHint.INFO, "INFO", "system", "Initialized " + instance.UniqueId + " plugin");
            }
        }

        private IEnumerable<Metric> GetMetrics()
        {
            var tab = this.tabControlPlugins.SelectedTab;
            if (tab == null)
            {
                return Enumerable.Empty<Metric>();
            }
            else
            {
                var view = tab.Tag as TreeView;
                var node = view.SelectedNode;
                if (node == null)
                {
                    return Enumerable.Empty<Metric>();
                }
                else
                {
                    return (node.Tag as ISimulatorNode).Metrics;                   
                }
            }
        }

        private void timerMetrics_Tick(object sender, EventArgs e)
        {
            var metrics = GetMetrics();
            this.listViewMetrics.SuspendLayout();
            this.listViewMetrics.Items.Clear();
            foreach (Metric m in metrics)
            {
                var values = new String[] { m.Id, m.Value };
                var item = new ListViewItem(values);
                this.listViewMetrics.Items.Add(item);
            }
            this.listViewMetrics.ResumeLayout();
        }

        private void SimulatorForm_Shown(object sender, EventArgs e)
        {
            if (splashOnLoad)
            {
                using (var about = new AboutBox())
                {
                    about.ShowDialog();
                }
            }             
        }

        private void SimulatorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            fileLogger.Shutdown();
        }

        private void buttonMakeNote_Click(object sender, EventArgs e)
        {
            using (var dialog = new MakeNoteDialog())
            {
                dialog.ShowDialog();
                if (dialog.DialogResult == DialogResult.OK)
                {
                    var lines = dialog.SelectedLines;
                    var first = lines.Take(1);
                    var remainder = lines.Skip(1);
                    
                    foreach (var line in first)
                    {
                        log.LogFull(DisplayHint.ALT2, "USER", "system", line);
                    }

                    foreach (var line in remainder)
                    {
                        log.Log(DisplayHint.ALT2, line);
                    }
                }
            }
        }

    }
}
