using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public partial class StackBrowser : UserControl
    {
        public delegate void AddTcpLayer(TcpSettings settings);
        public delegate void RemovePort(String name);
        public delegate void AddMaster(String name, String port, FilterLevel level, MasterStackConfig config);
        public delegate void RemoveStack(String name);
        public delegate void AddSerialPort(SerialSettings settings);

        public event AddTcpLayer OnTcpClientAdded;
        public event AddTcpLayer OnTcpServerAdded;
        public event AddSerialPort OnSerialPortAdded;
        public event RemovePort OnRemovePort;
        public event AddMaster OnAddMaster;
        public event RemoveStack OnRemoveStack;

        private TreeNode root = new TreeNode("root");

        private ContextMenuStrip WireNewPortMenuStrip(EventHandler onAddMasterClick, EventHandler onRemoveClick)
        {
            ContextMenuStrip menu = new ContextMenuStrip(this.components);
            var addMaster = new ToolStripMenuItem("Add Master");
            addMaster.Click += onAddMasterClick;
            var sep = new ToolStripSeparator();
            var remove = new ToolStripMenuItem("Remove");
            remove.Click += onRemoveClick;
            menu.Items.AddRange(new ToolStripItem[] { addMaster, sep, remove });
            return menu;
        }

        private ContextMenuStrip WireNewMasterMenuStrip(EventHandler onRemoveClick)
        {
            ContextMenuStrip menu = new ContextMenuStrip(this.components);
            var sep = new ToolStripSeparator();
            var remove = new ToolStripMenuItem("Remove");
            remove.Click += onRemoveClick;
            menu.Items.AddRange(new ToolStripItem[] { sep, remove });
            return menu;
        }


        public StackBrowser()
        {
            InitializeComponent();
            root.ContextMenuStrip = this.contextMenuStripAddComms;
            this.treeView.Nodes.Add(root);
            this.treeView.ShowNodeToolTips = true;
        }

        private void RemovePortClick(TreeNode node)
        {
            if (this.OnRemovePort != null) this.OnRemovePort(node.Text);
            root.Nodes.Remove(node);
        }

        private void AddMasterClick(TreeNode portNode)
        {
            MasterSettingsForm dialog = new MasterSettingsForm();
            dialog.OnAddMaster += new MasterSettingsForm.AddMaster((name, level, config) => dialog_OnAddMaster(name, level, config, portNode));
            dialog.Show();
        }

        private void RemoveStackClick(TreeNode port, TreeNode stack)
        {
            if (this.OnRemoveStack != null) this.OnRemoveStack(stack.Text);
            port.Nodes.Remove(stack);
        }

        void dialog_OnAddMaster(String name, FilterLevel level, MasterStackConfig config, TreeNode portNode)
        {
            if (this.OnAddMaster != null)
            {
                if (portNode.Nodes.ContainsKey(name))
                {
                    MessageBox.Show("Port already contains a stack called: " + name);
                }
                else
                {
                    this.OnAddMaster(name, portNode.Text, level, config);
                    TreeNode master = portNode.Nodes.Add(name, name);
                    EventHandler removeStackAction = (obj, e) => this.RemoveStackClick(portNode, master);
                    master.ContextMenuStrip = this.WireNewMasterMenuStrip(removeStackAction);
                }
            }
        }
        
        private void clientToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            var form = new TcpSettingsForm("Add TCP Client", "TCP Client 1");
            form.OnTcpAdded += new TcpSettingsForm.AddTcp(form_OnTcpClientAdded);
            form.Show();
        }

        private void serverToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            var form = new TcpSettingsForm("Add TCP Server", "TCP Server 1");
            form.OnTcpAdded += new TcpSettingsForm.AddTcp(form_OnTcpServerAdded);
            form.Show();
        }

        private void serialToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            var form = new SerialSettingsForm();
            form.SerialPortAdded += new SerialSettingsForm.OnSerialPortAdded(form_SerialPortAdded);
            form.Show();
        }

        void form_SerialPortAdded(SerialSettings settings)
        {
            if (this.OnSerialPortAdded != null)
            {
                if (root.Nodes.ContainsKey(settings.port)) //port names cannot be duplicates
                {
                    MessageBox.Show("A port with that name already exists: " + settings.port);
                }
                else
                {
                    this.OnSerialPortAdded(settings);
                    TreeNode node = this.root.Nodes.Add(settings.port, settings.port);
                    EventHandler addMasterAction = (obj, e) => this.AddMasterClick(node);
                    EventHandler removePortAction = (obj, e) => this.RemovePortClick(node);
                    node.ContextMenuStrip = this.WireNewPortMenuStrip(addMasterAction, removePortAction);
                    node.ToolTipText = settings.ToString();
                }
            }
        }

        void form_OnTcpServerAdded(TcpSettings settings)
        {
            if (this.OnTcpServerAdded != null)
            {
                if (root.Nodes.ContainsKey(settings.name)) //port names cannot be duplicates
                {
                    MessageBox.Show("A port with that name already exists: " + settings.name);
                }
                else
                {
                    this.OnTcpServerAdded(settings);
                    TreeNode node = this.root.Nodes.Add(settings.name, settings.name);
                    EventHandler addMasterAction = (obj, e) => this.AddMasterClick(node);
                    EventHandler removePortAction = (obj, e) => this.RemovePortClick(node);
                    node.ContextMenuStrip = this.WireNewPortMenuStrip(addMasterAction, removePortAction);
                    node.ToolTipText = settings.ToString();
                }
            }
        }

        void form_OnTcpClientAdded(TcpSettings settings)
        {
            if (this.OnTcpClientAdded != null)
            {
                if (root.Nodes.ContainsKey(settings.name)) //port names cannot be duplicates
                {
                    MessageBox.Show("A port with that name already exists: " + settings.name);
                }
                else
                {
                    this.OnTcpClientAdded(settings);
                    TreeNode node = this.root.Nodes.Add(settings.name, settings.name);
                    EventHandler addMasterAction = (obj, e) => this.AddMasterClick(node);
                    EventHandler removePortAction = (obj, e) => this.RemovePortClick(node);
                    node.ContextMenuStrip = this.WireNewPortMenuStrip(addMasterAction, removePortAction);
                    node.ToolTipText = settings.ToString();                                       
                }
            }



        }

        

        
    }
}
