namespace Automatak.Simulator.UI
{
    partial class LogWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LogWindow));
            this.imageListLog = new System.Windows.Forms.ImageList(this.components);
            this.panelVert = new System.Windows.Forms.Panel();
            this.buttonClear = new System.Windows.Forms.Button();
            this.vScrollBar = new System.Windows.Forms.VScrollBar();
            this.buttonClipboard = new System.Windows.Forms.Button();
            this.buttonPause = new System.Windows.Forms.Button();
            this.buttonPlay = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.pauseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resumeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.copyToClipboardToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.timerRefresh = new System.Windows.Forms.Timer(this.components);
            this.logControl = new Automatak.Simulator.UI.LogControl();
            this.panelVert.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageListLog
            // 
            this.imageListLog.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListLog.ImageStream")));
            this.imageListLog.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListLog.Images.SetKeyName(0, "play.ico");
            this.imageListLog.Images.SetKeyName(1, "pause.ico");
            this.imageListLog.Images.SetKeyName(2, "clipboard_diskette.ico");
            this.imageListLog.Images.SetKeyName(3, "log_cross.ico");
            // 
            // panelVert
            // 
            this.panelVert.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelVert.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelVert.Controls.Add(this.buttonClear);
            this.panelVert.Controls.Add(this.vScrollBar);
            this.panelVert.Controls.Add(this.buttonClipboard);
            this.panelVert.Controls.Add(this.buttonPause);
            this.panelVert.Controls.Add(this.buttonPlay);
            this.panelVert.Location = new System.Drawing.Point(813, 0);
            this.panelVert.Name = "panelVert";
            this.panelVert.Size = new System.Drawing.Size(40, 282);
            this.panelVert.TabIndex = 4;
            // 
            // buttonClear
            // 
            this.buttonClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClear.FlatAppearance.BorderColor = System.Drawing.SystemColors.Control;
            this.buttonClear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonClear.ImageIndex = 3;
            this.buttonClear.ImageList = this.imageListLog;
            this.buttonClear.Location = new System.Drawing.Point(3, 212);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(32, 32);
            this.buttonClear.TabIndex = 7;
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.buttonClear_Click);
            // 
            // vScrollBar
            // 
            this.vScrollBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.vScrollBar.Location = new System.Drawing.Point(3, -1);
            this.vScrollBar.Name = "vScrollBar";
            this.vScrollBar.Size = new System.Drawing.Size(32, 142);
            this.vScrollBar.TabIndex = 6;
            this.vScrollBar.Value = 100;
            this.vScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar_Scroll);
            // 
            // buttonClipboard
            // 
            this.buttonClipboard.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClipboard.FlatAppearance.BorderColor = System.Drawing.SystemColors.Control;
            this.buttonClipboard.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonClipboard.ImageIndex = 2;
            this.buttonClipboard.ImageList = this.imageListLog;
            this.buttonClipboard.Location = new System.Drawing.Point(3, 246);
            this.buttonClipboard.Name = "buttonClipboard";
            this.buttonClipboard.Size = new System.Drawing.Size(32, 32);
            this.buttonClipboard.TabIndex = 5;
            this.buttonClipboard.UseVisualStyleBackColor = true;
            this.buttonClipboard.Click += new System.EventHandler(this.buttonClipboard_Click);
            // 
            // buttonPause
            // 
            this.buttonPause.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPause.FlatAppearance.BorderColor = System.Drawing.SystemColors.Control;
            this.buttonPause.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonPause.ImageIndex = 1;
            this.buttonPause.ImageList = this.imageListLog;
            this.buttonPause.Location = new System.Drawing.Point(3, 177);
            this.buttonPause.Name = "buttonPause";
            this.buttonPause.Size = new System.Drawing.Size(32, 32);
            this.buttonPause.TabIndex = 4;
            this.buttonPause.UseVisualStyleBackColor = true;
            this.buttonPause.Click += new System.EventHandler(this.buttonPause_Click);
            // 
            // buttonPlay
            // 
            this.buttonPlay.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPlay.Enabled = false;
            this.buttonPlay.FlatAppearance.BorderColor = System.Drawing.SystemColors.Control;
            this.buttonPlay.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.buttonPlay.ImageIndex = 0;
            this.buttonPlay.ImageList = this.imageListLog;
            this.buttonPlay.Location = new System.Drawing.Point(3, 144);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(32, 32);
            this.buttonPlay.TabIndex = 3;
            this.buttonPlay.UseVisualStyleBackColor = true;
            this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 281);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(853, 26);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 5;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Padding = new System.Windows.Forms.Padding(10, 3, 3, 3);
            this.toolStripStatusLabel.Size = new System.Drawing.Size(83, 21);
            this.toolStripStatusLabel.Text = "Hello World";
            // 
            // contextMenuStrip
            // 
            this.contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.pauseToolStripMenuItem,
            this.resumeToolStripMenuItem,
            this.toolStripMenuItem1,
            this.clearToolStripMenuItem,
            this.copyToClipboardToolStripMenuItem});
            this.contextMenuStrip.Name = "contextMenuStrip";
            this.contextMenuStrip.Size = new System.Drawing.Size(172, 98);
            // 
            // pauseToolStripMenuItem
            // 
            this.pauseToolStripMenuItem.Name = "pauseToolStripMenuItem";
            this.pauseToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            this.pauseToolStripMenuItem.Text = "Pause";
            this.pauseToolStripMenuItem.Click += new System.EventHandler(this.pauseToolStripMenuItem_Click_1);
            // 
            // resumeToolStripMenuItem
            // 
            this.resumeToolStripMenuItem.Enabled = false;
            this.resumeToolStripMenuItem.Name = "resumeToolStripMenuItem";
            this.resumeToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            this.resumeToolStripMenuItem.Text = "Resume";
            this.resumeToolStripMenuItem.Click += new System.EventHandler(this.resumeToolStripMenuItem_Click_1);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(168, 6);
            // 
            // clearToolStripMenuItem
            // 
            this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
            this.clearToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            this.clearToolStripMenuItem.Text = "Clear";
            this.clearToolStripMenuItem.Click += new System.EventHandler(this.clearToolStripMenuItem_Click);
            // 
            // copyToClipboardToolStripMenuItem
            // 
            this.copyToClipboardToolStripMenuItem.Name = "copyToClipboardToolStripMenuItem";
            this.copyToClipboardToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            this.copyToClipboardToolStripMenuItem.Text = "Copy to Clipboard";
            this.copyToClipboardToolStripMenuItem.Click += new System.EventHandler(this.copyToClipboardToolStripMenuItem_Click);
            // 
            // timerRefresh
            // 
            this.timerRefresh.Enabled = true;
            this.timerRefresh.Interval = 250;
            this.timerRefresh.Tick += new System.EventHandler(this.timerRefresh_Tick);
            // 
            // logControl
            // 
            this.logControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.logControl.AutoScroll = true;
            this.logControl.AutoScrollMinSize = new System.Drawing.Size(100, 100);
            this.logControl.BackColor = System.Drawing.Color.DimGray;
            this.logControl.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.logControl.ContextMenuStrip = this.contextMenuStrip;
            this.logControl.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.logControl.ForeColor = System.Drawing.SystemColors.Window;
            this.logControl.Location = new System.Drawing.Point(0, 0);
            this.logControl.Margin = new System.Windows.Forms.Padding(0);
            this.logControl.MinimumSize = new System.Drawing.Size(500, 150);
            this.logControl.Name = "logControl";
            this.logControl.Size = new System.Drawing.Size(814, 282);
            this.logControl.SpacingFactor = 0F;
            this.logControl.TabIndex = 0;
            // 
            // LogWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.panelVert);
            this.Controls.Add(this.logControl);
            this.MinimumSize = new System.Drawing.Size(300, 250);
            this.Name = "LogWindow";
            this.Size = new System.Drawing.Size(853, 307);
            this.panelVert.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.contextMenuStrip.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private LogControl logControl;
        private System.Windows.Forms.ImageList imageListLog;
        private System.Windows.Forms.Panel panelVert;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.Button buttonPlay;
        private System.Windows.Forms.Button buttonPause;
        private System.Windows.Forms.Button buttonClipboard;
        private System.Windows.Forms.VScrollBar vScrollBar;
        private System.Windows.Forms.Button buttonClear;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem pauseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem resumeToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem copyToClipboardToolStripMenuItem;
        private System.Windows.Forms.Timer timerRefresh;
    }
}
