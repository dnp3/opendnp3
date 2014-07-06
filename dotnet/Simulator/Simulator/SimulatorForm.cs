using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.API;

namespace Automatak.Simulator
{
    public partial class SimulatorForm : Form
    {
        readonly IEnumerable<ISimulatorPluginFactory> plugins;

        public SimulatorForm(IEnumerable<ISimulatorPluginFactory> plugins)
        {                 
            InitializeComponent();    
            
            this.plugins = plugins;     
        }
           
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
                var instance = factory.Create(this.logWindow1);                
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
            }
        }

        private void timerMetrics_Tick(object sender, EventArgs e)
        {
            var tab = this.tabControlPlugins.SelectedTab;
            if (tab != null)
            {
                var view = tab.Tag as TreeView;
                var node = view.SelectedNode;
                if (node != null)
                { 
                    var metrics = (node.Tag as ISimulatorNode).Metrics;
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
            }
        }

    }
}
