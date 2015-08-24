using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    partial class MasterScanControl
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
            this.listViewScans = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.contextMenuStripAdd = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addScanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStripAdd.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewScans
            // 
            this.listViewScans.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewScans.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listViewScans.FullRowSelect = true;
            this.listViewScans.Location = new System.Drawing.Point(0, 0);
            this.listViewScans.MultiSelect = false;
            this.listViewScans.Name = "listViewScans";
            this.listViewScans.ShowItemToolTips = true;
            this.listViewScans.Size = new System.Drawing.Size(420, 481);
            this.listViewScans.TabIndex = 0;
            this.listViewScans.UseCompatibleStateImageBehavior = false;
            this.listViewScans.View = System.Windows.Forms.View.Details;
            this.listViewScans.MouseDown += new System.Windows.Forms.MouseEventHandler(this.listViewScans_MouseDown);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Type";
            this.columnHeader1.Width = 93;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Period";
            this.columnHeader2.Width = 132;
            // 
            // contextMenuStripAdd
            // 
            this.contextMenuStripAdd.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addScanToolStripMenuItem});
            this.contextMenuStripAdd.Name = "contextMenuStripAdd";
            this.contextMenuStripAdd.Size = new System.Drawing.Size(125, 26);
            // 
            // addScanToolStripMenuItem
            // 
            this.addScanToolStripMenuItem.Name = "addScanToolStripMenuItem";
            this.addScanToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
            this.addScanToolStripMenuItem.Text = "Add Scan";
            this.addScanToolStripMenuItem.Click += new System.EventHandler(this.addScanToolStripMenuItem_Click);
            // 
            // MasterScanControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.listViewScans);
            this.Name = "MasterScanControl";
            this.Size = new System.Drawing.Size(420, 481);
            this.contextMenuStripAdd.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewScans;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripAdd;
        private System.Windows.Forms.ToolStripMenuItem addScanToolStripMenuItem;

    }
}
