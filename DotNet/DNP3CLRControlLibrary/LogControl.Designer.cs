namespace DotNetTestSet
{
    partial class LogControl
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
            this.tabControlLog = new System.Windows.Forms.TabControl();
            this.tabPageView = new System.Windows.Forms.TabPage();
            this.tabPageOptions = new System.Windows.Forms.TabPage();
            this.flickerFreeListView1 = new TestSetControlLibrary.FlickerFreeListView();
            this.columnHeaderLevel = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderTime = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderMessage = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabControlLog.SuspendLayout();
            this.tabPageView.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControlLog
            // 
            this.tabControlLog.Controls.Add(this.tabPageView);
            this.tabControlLog.Controls.Add(this.tabPageOptions);
            this.tabControlLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlLog.Location = new System.Drawing.Point(0, 0);
            this.tabControlLog.Name = "tabControlLog";
            this.tabControlLog.SelectedIndex = 0;
            this.tabControlLog.Size = new System.Drawing.Size(775, 246);
            this.tabControlLog.TabIndex = 0;
            // 
            // tabPageView
            // 
            this.tabPageView.Controls.Add(this.flickerFreeListView1);
            this.tabPageView.Location = new System.Drawing.Point(4, 22);
            this.tabPageView.Name = "tabPageView";
            this.tabPageView.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageView.Size = new System.Drawing.Size(767, 220);
            this.tabPageView.TabIndex = 0;
            this.tabPageView.Text = "Messages";
            this.tabPageView.UseVisualStyleBackColor = true;
            // 
            // tabPageOptions
            // 
            this.tabPageOptions.Location = new System.Drawing.Point(4, 22);
            this.tabPageOptions.Name = "tabPageOptions";
            this.tabPageOptions.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageOptions.Size = new System.Drawing.Size(767, 220);
            this.tabPageOptions.TabIndex = 1;
            this.tabPageOptions.Text = "Options";
            this.tabPageOptions.UseVisualStyleBackColor = true;
            // 
            // flickerFreeListView1
            // 
            this.flickerFreeListView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderLevel,
            this.columnHeaderTime,
            this.columnHeaderMessage});
            this.flickerFreeListView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flickerFreeListView1.Location = new System.Drawing.Point(3, 3);
            this.flickerFreeListView1.Name = "flickerFreeListView1";
            this.flickerFreeListView1.Size = new System.Drawing.Size(761, 214);
            this.flickerFreeListView1.TabIndex = 0;
            this.flickerFreeListView1.UseCompatibleStateImageBehavior = false;
            this.flickerFreeListView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeaderLevel
            // 
            this.columnHeaderLevel.Text = "Level";
            // 
            // columnHeaderTime
            // 
            this.columnHeaderTime.Text = "Time";
            // 
            // columnHeaderMessage
            // 
            this.columnHeaderMessage.Text = "Message";
            this.columnHeaderMessage.Width = 644;
            // 
            // LogControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControlLog);
            this.Name = "LogControl";
            this.Size = new System.Drawing.Size(775, 246);
            this.tabControlLog.ResumeLayout(false);
            this.tabPageView.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlLog;
        private System.Windows.Forms.TabPage tabPageView;
        private System.Windows.Forms.TabPage tabPageOptions;
        private TestSetControlLibrary.FlickerFreeListView flickerFreeListView1;
        private System.Windows.Forms.ColumnHeader columnHeaderLevel;
        private System.Windows.Forms.ColumnHeader columnHeaderTime;
        private System.Windows.Forms.ColumnHeader columnHeaderMessage;
    }
}
