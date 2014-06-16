using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    public partial class CommTreeView : UserControl
    {
        public CommTreeView()
        {
            InitializeComponent();                     
        }

        public void AddChannel(string id, IChannel channel, IEnumerable<IMasterPluginFactory> masters)
        {
            TreeNode node = new TreeNode(id);            
            node.ImageIndex = 0;
            node.ContextMenuStrip = CreateChannelMenuStrip(node, channel, masters);
            node.Tag = channel;
            treeView.Nodes.Add(node);            
        }

        ContextMenuStrip CreateChannelMenuStrip(TreeNode node, IChannel channel, IEnumerable<IMasterPluginFactory> masters)
        {
            var strip = new ContextMenuStrip();
            var add = new ToolStripMenuItem("Add");
            strip.Items.Add(add);

            foreach(var plugin in masters)
            {
                var item =  add.DropDownItems.Add(plugin.Name);
                item.Click += (object sender, EventArgs e) => OnAddMaster(plugin, node, channel, sender, e);
            }
                        
            strip.Items.Add(new ToolStripSeparator());
            var remove = strip.Items.Add("Remove");
            remove.Click += (object sender, EventArgs e) => OnRemoveChannel(node, channel, sender, e);                        
            return strip;
        }

        ContextMenuStrip CreateMasterMenuStrip(IMasterPlugin plugin, TreeNode parent, TreeNode node, IMaster master)
        {
            var strip = new ContextMenuStrip();

            if (plugin.PluginForm != null)
            {
                var open = strip.Items.Add("Open");
                open.Click += (object sender, EventArgs e) => plugin.PluginForm.Show();
                strip.Items.Add(new ToolStripSeparator());
            }
            
            var remove = strip.Items.Add("Remove");
            remove.Click += (object sender, EventArgs e) => OnRemoveMaster(parent, node, master, sender, e);
            return strip;
        }

        private void OnRemoveMaster(TreeNode parent, TreeNode node, IMaster master, object sender, EventArgs e)
        {
            parent.Nodes.Remove(node);
            master.Shutdown();
        }

        private void OnAddMaster(IMasterPluginFactory factory, TreeNode node, IChannel channel, object sender, EventArgs e)
        {
            using (MasterDialog dialog = new MasterDialog())
            {
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    var plugin = factory.Create("master");
                    var master = channel.AddMaster("master", plugin.SOEHandler, dialog.Configuration);
                    if (master != null)
                    {
                        plugin.SetMaster(master);
                        TreeNode masterNode = new TreeNode("master");
                        masterNode.ImageIndex = 1;
                        masterNode.StateImageIndex = 1;
                        masterNode.SelectedImageIndex = 1;
                        masterNode.ContextMenuStrip = CreateMasterMenuStrip(plugin, node, masterNode, master);
                        node.Nodes.Add(masterNode);
                        master.Enable();
                    }
                }
            }
        }

        private void OnRemoveChannel(TreeNode node, IChannel channel, object sender, EventArgs e)
        {
            treeView.Nodes.Remove(node);
            channel.Shutdown();           
        }      

        private IEnumerable<string> ToLines(IChannelStatistics stats)
        {
            yield return "Num bytes rx: " + stats.NumBytesRx;
            yield return "Num bytes tx: " + stats.NumBytesTx;
            yield return "Num open: " + stats.NumOpen;
            yield return "Num open fail: " + stats.NumOpenFail;
            yield return "Num close: " + stats.NumClose;
            yield return "Num crc error: " + stats.NumCrcError;
            yield return "Num bad lpdu: " + stats.NumBadLinkFrame;

        }
    }
}
