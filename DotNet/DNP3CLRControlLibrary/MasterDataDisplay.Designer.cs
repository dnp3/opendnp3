namespace TestSetControlLibrary
{
    partial class MasterDataDisplay
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
            this.tabPageAOStatus = new System.Windows.Forms.TabPage();
            this.tabPageBOStatus = new System.Windows.Forms.TabPage();
            this.tabPageCounter = new System.Windows.Forms.TabPage();
            this.tabPageAnalog = new System.Windows.Forms.TabPage();
            this.tabPageBinary = new System.Windows.Forms.TabPage();
            this.tabControlMeas = new System.Windows.Forms.TabControl();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.issueToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.analogOutputToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.binaryOutputToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.flickerFreeListViewBinary = new TestSetControlLibrary.FlickerFreeListView();
            this.columnHeaderIndex = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.flickerFreeListViewAnalog = new TestSetControlLibrary.FlickerFreeListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.flickerFreeListViewCounter = new TestSetControlLibrary.FlickerFreeListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.flickerFreeListViewBOStatus = new TestSetControlLibrary.FlickerFreeListView();
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.flickerFreeListViewAOStatus = new TestSetControlLibrary.FlickerFreeListView();
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader8 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPageAOStatus.SuspendLayout();
            this.tabPageBOStatus.SuspendLayout();
            this.tabPageCounter.SuspendLayout();
            this.tabPageAnalog.SuspendLayout();
            this.tabPageBinary.SuspendLayout();
            this.tabControlMeas.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabPageAOStatus
            // 
            this.tabPageAOStatus.Controls.Add(this.flickerFreeListViewAOStatus);
            this.tabPageAOStatus.Location = new System.Drawing.Point(4, 22);
            this.tabPageAOStatus.Name = "tabPageAOStatus";
            this.tabPageAOStatus.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageAOStatus.Size = new System.Drawing.Size(830, 475);
            this.tabPageAOStatus.TabIndex = 4;
            this.tabPageAOStatus.Text = "Analog Output Status";
            this.tabPageAOStatus.UseVisualStyleBackColor = true;
            // 
            // tabPageBOStatus
            // 
            this.tabPageBOStatus.Controls.Add(this.flickerFreeListViewBOStatus);
            this.tabPageBOStatus.Location = new System.Drawing.Point(4, 22);
            this.tabPageBOStatus.Name = "tabPageBOStatus";
            this.tabPageBOStatus.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBOStatus.Size = new System.Drawing.Size(830, 475);
            this.tabPageBOStatus.TabIndex = 3;
            this.tabPageBOStatus.Text = "Binary Output Status";
            this.tabPageBOStatus.UseVisualStyleBackColor = true;
            // 
            // tabPageCounter
            // 
            this.tabPageCounter.Controls.Add(this.flickerFreeListViewCounter);
            this.tabPageCounter.Location = new System.Drawing.Point(4, 22);
            this.tabPageCounter.Name = "tabPageCounter";
            this.tabPageCounter.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCounter.Size = new System.Drawing.Size(830, 475);
            this.tabPageCounter.TabIndex = 2;
            this.tabPageCounter.Text = "Counter";
            this.tabPageCounter.UseVisualStyleBackColor = true;
            // 
            // tabPageAnalog
            // 
            this.tabPageAnalog.Controls.Add(this.flickerFreeListViewAnalog);
            this.tabPageAnalog.Location = new System.Drawing.Point(4, 22);
            this.tabPageAnalog.Name = "tabPageAnalog";
            this.tabPageAnalog.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageAnalog.Size = new System.Drawing.Size(830, 475);
            this.tabPageAnalog.TabIndex = 1;
            this.tabPageAnalog.Text = "Analog Input";
            this.tabPageAnalog.UseVisualStyleBackColor = true;
            // 
            // tabPageBinary
            // 
            this.tabPageBinary.Controls.Add(this.flickerFreeListViewBinary);
            this.tabPageBinary.Location = new System.Drawing.Point(4, 22);
            this.tabPageBinary.Name = "tabPageBinary";
            this.tabPageBinary.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBinary.Size = new System.Drawing.Size(830, 475);
            this.tabPageBinary.TabIndex = 0;
            this.tabPageBinary.Text = "Binary Input";
            this.tabPageBinary.UseVisualStyleBackColor = true;
            // 
            // tabControlMeas
            // 
            this.tabControlMeas.Controls.Add(this.tabPageBinary);
            this.tabControlMeas.Controls.Add(this.tabPageAnalog);
            this.tabControlMeas.Controls.Add(this.tabPageCounter);
            this.tabControlMeas.Controls.Add(this.tabPageBOStatus);
            this.tabControlMeas.Controls.Add(this.tabPageAOStatus);
            this.tabControlMeas.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlMeas.Location = new System.Drawing.Point(0, 24);
            this.tabControlMeas.Name = "tabControlMeas";
            this.tabControlMeas.SelectedIndex = 0;
            this.tabControlMeas.Size = new System.Drawing.Size(838, 501);
            this.tabControlMeas.TabIndex = 0;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.issueToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.menuStrip1.Size = new System.Drawing.Size(838, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // issueToolStripMenuItem
            // 
            this.issueToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.analogOutputToolStripMenuItem,
            this.binaryOutputToolStripMenuItem});
            this.issueToolStripMenuItem.Name = "issueToolStripMenuItem";
            this.issueToolStripMenuItem.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.issueToolStripMenuItem.Size = new System.Drawing.Size(45, 20);
            this.issueToolStripMenuItem.Text = "Issue";
            // 
            // analogOutputToolStripMenuItem
            // 
            this.analogOutputToolStripMenuItem.Name = "analogOutputToolStripMenuItem";
            this.analogOutputToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.analogOutputToolStripMenuItem.Text = "Analog Output";
            this.analogOutputToolStripMenuItem.Click += new System.EventHandler(this.analogOutputToolStripMenuItem_Click);
            // 
            // binaryOutputToolStripMenuItem
            // 
            this.binaryOutputToolStripMenuItem.Name = "binaryOutputToolStripMenuItem";
            this.binaryOutputToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.binaryOutputToolStripMenuItem.Text = "Binary Output";
            // 
            // flickerFreeListViewBinary
            // 
            this.flickerFreeListViewBinary.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flickerFreeListViewBinary.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderIndex,
            this.columnHeaderValue});
            this.flickerFreeListViewBinary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flickerFreeListViewBinary.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.flickerFreeListViewBinary.Location = new System.Drawing.Point(3, 3);
            this.flickerFreeListViewBinary.MultiSelect = false;
            this.flickerFreeListViewBinary.Name = "flickerFreeListViewBinary";
            this.flickerFreeListViewBinary.Size = new System.Drawing.Size(824, 469);
            this.flickerFreeListViewBinary.TabIndex = 0;
            this.flickerFreeListViewBinary.UseCompatibleStateImageBehavior = false;
            this.flickerFreeListViewBinary.View = System.Windows.Forms.View.Details;
            // 
            // columnHeaderIndex
            // 
            this.columnHeaderIndex.Text = "Index";
            // 
            // columnHeaderValue
            // 
            this.columnHeaderValue.Text = "Value";
            // 
            // flickerFreeListViewAnalog
            // 
            this.flickerFreeListViewAnalog.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flickerFreeListViewAnalog.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.flickerFreeListViewAnalog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flickerFreeListViewAnalog.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.flickerFreeListViewAnalog.Location = new System.Drawing.Point(3, 3);
            this.flickerFreeListViewAnalog.MultiSelect = false;
            this.flickerFreeListViewAnalog.Name = "flickerFreeListViewAnalog";
            this.flickerFreeListViewAnalog.Size = new System.Drawing.Size(824, 469);
            this.flickerFreeListViewAnalog.TabIndex = 1;
            this.flickerFreeListViewAnalog.UseCompatibleStateImageBehavior = false;
            this.flickerFreeListViewAnalog.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Index";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Value";
            // 
            // flickerFreeListViewCounter
            // 
            this.flickerFreeListViewCounter.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flickerFreeListViewCounter.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
            this.flickerFreeListViewCounter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flickerFreeListViewCounter.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.flickerFreeListViewCounter.Location = new System.Drawing.Point(3, 3);
            this.flickerFreeListViewCounter.MultiSelect = false;
            this.flickerFreeListViewCounter.Name = "flickerFreeListViewCounter";
            this.flickerFreeListViewCounter.Size = new System.Drawing.Size(824, 469);
            this.flickerFreeListViewCounter.TabIndex = 2;
            this.flickerFreeListViewCounter.UseCompatibleStateImageBehavior = false;
            this.flickerFreeListViewCounter.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Index";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "Value";
            // 
            // flickerFreeListViewBOStatus
            // 
            this.flickerFreeListViewBOStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flickerFreeListViewBOStatus.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader5,
            this.columnHeader6});
            this.flickerFreeListViewBOStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flickerFreeListViewBOStatus.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.flickerFreeListViewBOStatus.Location = new System.Drawing.Point(3, 3);
            this.flickerFreeListViewBOStatus.MultiSelect = false;
            this.flickerFreeListViewBOStatus.Name = "flickerFreeListViewBOStatus";
            this.flickerFreeListViewBOStatus.Size = new System.Drawing.Size(824, 469);
            this.flickerFreeListViewBOStatus.TabIndex = 3;
            this.flickerFreeListViewBOStatus.UseCompatibleStateImageBehavior = false;
            this.flickerFreeListViewBOStatus.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Index";
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "Value";
            // 
            // flickerFreeListViewAOStatus
            // 
            this.flickerFreeListViewAOStatus.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.flickerFreeListViewAOStatus.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader7,
            this.columnHeader8});
            this.flickerFreeListViewAOStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flickerFreeListViewAOStatus.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.flickerFreeListViewAOStatus.Location = new System.Drawing.Point(3, 3);
            this.flickerFreeListViewAOStatus.MultiSelect = false;
            this.flickerFreeListViewAOStatus.Name = "flickerFreeListViewAOStatus";
            this.flickerFreeListViewAOStatus.Size = new System.Drawing.Size(824, 469);
            this.flickerFreeListViewAOStatus.TabIndex = 4;
            this.flickerFreeListViewAOStatus.UseCompatibleStateImageBehavior = false;
            this.flickerFreeListViewAOStatus.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "Index";
            // 
            // columnHeader8
            // 
            this.columnHeader8.Text = "Value";
            // 
            // MasterDataDisplay
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControlMeas);
            this.Controls.Add(this.menuStrip1);
            this.Name = "MasterDataDisplay";
            this.Size = new System.Drawing.Size(838, 525);
            this.tabPageAOStatus.ResumeLayout(false);
            this.tabPageBOStatus.ResumeLayout(false);
            this.tabPageCounter.ResumeLayout(false);
            this.tabPageAnalog.ResumeLayout(false);
            this.tabPageBinary.ResumeLayout(false);
            this.tabControlMeas.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabPage tabPageAOStatus;
        private FlickerFreeListView flickerFreeListViewAOStatus;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.ColumnHeader columnHeader8;
        private System.Windows.Forms.TabPage tabPageBOStatus;
        private FlickerFreeListView flickerFreeListViewBOStatus;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.TabPage tabPageCounter;
        private FlickerFreeListView flickerFreeListViewCounter;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.TabPage tabPageAnalog;
        private FlickerFreeListView flickerFreeListViewAnalog;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.TabPage tabPageBinary;
        private FlickerFreeListView flickerFreeListViewBinary;
        private System.Windows.Forms.ColumnHeader columnHeaderIndex;
        private System.Windows.Forms.ColumnHeader columnHeaderValue;
        private System.Windows.Forms.TabControl tabControlMeas;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem issueToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem analogOutputToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem binaryOutputToolStripMenuItem;


    }
}
