namespace TestSetControlLibrary
{
    partial class StackBrowser
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.contextMenuStripAddComms = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.tCPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.clientToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.serverToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.serialToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.treeView = new System.Windows.Forms.TreeView();
            this.contextMenuStripAddComms.SuspendLayout();
            this.SuspendLayout();
            // 
            // contextMenuStripAddComms
            // 
            this.contextMenuStripAddComms.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tCPToolStripMenuItem,
            this.serialToolStripMenuItem1});
            this.contextMenuStripAddComms.Name = "contextMenuStripAddComms";
            this.contextMenuStripAddComms.Size = new System.Drawing.Size(153, 70);
            // 
            // tCPToolStripMenuItem
            // 
            this.tCPToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clientToolStripMenuItem1,
            this.serverToolStripMenuItem1});
            this.tCPToolStripMenuItem.Name = "tCPToolStripMenuItem";
            this.tCPToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.tCPToolStripMenuItem.Text = "TCP";
            // 
            // clientToolStripMenuItem1
            // 
            this.clientToolStripMenuItem1.Name = "clientToolStripMenuItem1";
            this.clientToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.clientToolStripMenuItem1.Text = "Client";
            this.clientToolStripMenuItem1.Click += new System.EventHandler(this.clientToolStripMenuItem1_Click);
            // 
            // serverToolStripMenuItem1
            // 
            this.serverToolStripMenuItem1.Name = "serverToolStripMenuItem1";
            this.serverToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.serverToolStripMenuItem1.Text = "Server";
            this.serverToolStripMenuItem1.Click += new System.EventHandler(this.serverToolStripMenuItem1_Click);
            // 
            // serialToolStripMenuItem1
            // 
            this.serialToolStripMenuItem1.Name = "serialToolStripMenuItem1";
            this.serialToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.serialToolStripMenuItem1.Text = "Serial";
            this.serialToolStripMenuItem1.Click += new System.EventHandler(this.serialToolStripMenuItem1_Click);
            // 
            // treeView
            // 
            this.treeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.treeView.Location = new System.Drawing.Point(0, 0);
            this.treeView.Name = "treeView";
            this.treeView.Size = new System.Drawing.Size(259, 418);
            this.treeView.TabIndex = 0;
            // 
            // StackBrowser
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.treeView);
            this.Name = "StackBrowser";
            this.Size = new System.Drawing.Size(259, 418);
            this.contextMenuStripAddComms.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TreeView treeView;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripAddComms;
        private System.Windows.Forms.ToolStripMenuItem tCPToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem clientToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem serverToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem serialToolStripMenuItem1;
    }
}
