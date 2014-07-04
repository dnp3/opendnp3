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

            var menu = new ContextMenu();
            var item = new MenuItem("Remove");
            item.Click += new EventHandler(
                delegate(Object o, EventArgs a)
                {
                    simNode.Remove();
                    parent.Remove(node);
                }
            );

            menu.MenuItems.Add("-");
            menu.MenuItems.Add(item);
            node.ContextMenu = menu;
            parent.Add(node);
        }
        
        private void SimulatorForm_Load(object sender, EventArgs e)
        {            
            foreach (var factory in plugins)
            {
                var instance = factory.Create();                
                var item = new ToolStripMenuItem(instance.RootDisplayName);
                item.Image = instance.PluginImage;
                this.addToolStripMenuItem.DropDownItems.Add(item);
                var page = new TabPage(instance.UniqueId);
                var treeView = new TreeView();
                treeView.Dock = DockStyle.Fill;
                treeView.ImageList = instance.NodeImageList;
                page.Controls.Add(treeView);
                this.tabControlPlugins.TabPages.Add(page);
                
                item.Click += new EventHandler(
                    delegate(Object o, EventArgs a)
                    {                        
                        var callbacks = new TreeNodeCallbcks();
                        var node = instance.Create(callbacks);
                        if (node != null)
                        {
                            BindNode(node, callbacks.node, treeView.Nodes);
                        }
                    }
                );
            }
        }

    }
}
