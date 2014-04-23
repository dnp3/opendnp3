using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace DNP3TestHarness
{
    public partial class CommTreeView : UserControl
    {
        public CommTreeView()
        {
            InitializeComponent();                     
        }

        public void AddChannel(string id, IChannel channel)
        {
            TreeNode node = new TreeNode(id);            
            node.ImageIndex = 0;
            node.ContextMenuStrip = CreateChannelMenuStrip(node, channel);
            treeView.Nodes.Add(node);         
        }

        ContextMenuStrip CreateChannelMenuStrip(TreeNode node, IChannel channel)
        {
            var strip = new ContextMenuStrip();
            var addMaster = strip.Items.Add("Add Master");
            addMaster.Click += (object sender, EventArgs e) => OnAddMaster(node, channel, sender, e);
            strip.Items.Add(new ToolStripSeparator());
            var remove = strip.Items.Add("Remove");
            remove.Click += (object sender, EventArgs e) => OnRemoveChannel(node, channel, sender, e);                        
            return strip;
        }

        ContextMenuStrip CreateMasterMenuStrip(TreeNode parent, TreeNode node, IMaster master)
        {
            var strip = new ContextMenuStrip();
            var remove = strip.Items.Add("Remove");
            remove.Click += (object sender, EventArgs e) => OnRemoveMaster(parent, node, master, sender, e);
            return strip;
        }

        private void OnRemoveMaster(TreeNode parent, TreeNode node, IMaster master, object sender, EventArgs e)
        {
            parent.Nodes.Remove(node);
            master.Shutdown();
        }

        private void OnAddMaster(TreeNode node, IChannel channel, object sender, EventArgs e)
        {
            using (MasterDialog dialog = new MasterDialog())
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    var master = channel.AddMaster("master", PrintingSOEHandler.Instance, dialog.Configuration);
                    TreeNode masterNode = new TreeNode("master");
                    masterNode.ImageIndex = 1;
                    masterNode.StateImageIndex = 1;
                    masterNode.SelectedImageIndex = 1;
                    masterNode.ContextMenuStrip = CreateMasterMenuStrip(node, masterNode, master);
                    node.Nodes.Add(masterNode);
                    master.Enable();
                }
            }
        }

        private void OnRemoveChannel(TreeNode node, IChannel channel, object sender, EventArgs e)
        {
            treeView.Nodes.Remove(node);
            channel.Shutdown();           
        }
    }
}
