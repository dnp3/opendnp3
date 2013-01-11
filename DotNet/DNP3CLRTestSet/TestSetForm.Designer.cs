namespace DotNetTestSet
{
    partial class TestSetForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.chartsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addNewChartToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.stackBrowser1 = new TestSetControlLibrary.StackBrowser();
            this.logControl = new DotNetTestSet.LogControl();
            this.stackDisplayControl = new TestSetControlLibrary.StackDisplayControl();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 24);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer2);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.logControl);
            this.splitContainer1.Size = new System.Drawing.Size(1052, 522);
            this.splitContainer1.SplitterDistance = 369;
            this.splitContainer1.TabIndex = 0;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.stackBrowser1);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.stackDisplayControl);
            this.splitContainer2.Size = new System.Drawing.Size(1052, 369);
            this.splitContainer2.SplitterDistance = 227;
            this.splitContainer2.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.chartsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1052, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // chartsToolStripMenuItem
            // 
            this.chartsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addNewChartToolStripMenuItem});
            this.chartsToolStripMenuItem.Name = "chartsToolStripMenuItem";
            this.chartsToolStripMenuItem.Size = new System.Drawing.Size(51, 20);
            this.chartsToolStripMenuItem.Text = "Charts";
            // 
            // addNewChartToolStripMenuItem
            // 
            this.addNewChartToolStripMenuItem.Name = "addNewChartToolStripMenuItem";
            this.addNewChartToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.addNewChartToolStripMenuItem.Text = "Add New Chart";
            
            // 
            // stackBrowser1
            // 
            this.stackBrowser1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.stackBrowser1.Location = new System.Drawing.Point(0, 0);
            this.stackBrowser1.Name = "stackBrowser1";
            this.stackBrowser1.Size = new System.Drawing.Size(227, 369);
            this.stackBrowser1.TabIndex = 0;
            this.stackBrowser1.OnTcpClientAdded += new TestSetControlLibrary.StackBrowser.AddTcpLayer(this.stackBrowser1_OnTcpClientAdded);
            this.stackBrowser1.OnTcpServerAdded += new TestSetControlLibrary.StackBrowser.AddTcpLayer(this.stackBrowser1_OnTcpServerAdded);
            this.stackBrowser1.OnSerialPortAdded += new TestSetControlLibrary.StackBrowser.AddSerialPort(this.stackBrowser1_OnSerialPortAdded);
            this.stackBrowser1.OnRemovePort += new TestSetControlLibrary.StackBrowser.RemovePort(this.stackBrowser1_OnRemovePort);
            this.stackBrowser1.OnAddMaster += new TestSetControlLibrary.StackBrowser.AddMaster(this.stackBrowser1_OnAddMaster);
            this.stackBrowser1.OnRemoveStack += new TestSetControlLibrary.StackBrowser.RemoveStack(this.stackBrowser1_OnRemoveStack);
            // 
            // logControl
            // 
            this.logControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.logControl.Location = new System.Drawing.Point(0, 0);
            this.logControl.Name = "logControl";
            this.logControl.Padding = new System.Windows.Forms.Padding(3);
            this.logControl.Size = new System.Drawing.Size(1052, 149);
            this.logControl.TabIndex = 0;
            // 
            // stackDisplayControl
            // 
            this.stackDisplayControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.stackDisplayControl.Location = new System.Drawing.Point(0, 0);
            this.stackDisplayControl.Name = "stackDisplayControl";
            this.stackDisplayControl.Size = new System.Drawing.Size(821, 369);
            this.stackDisplayControl.TabIndex = 0;
            // 
            // TestSetForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1052, 546);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "TestSetForm";
            this.Text = "TestSet";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private LogControl logControl;
        private TestSetControlLibrary.StackBrowser stackBrowser1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem chartsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addNewChartToolStripMenuItem;
        private TestSetControlLibrary.StackDisplayControl stackDisplayControl;
    }
}

