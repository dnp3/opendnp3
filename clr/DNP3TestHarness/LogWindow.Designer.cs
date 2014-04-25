namespace Automatak.DNP3.Simulator
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
            this.buttonUp = new System.Windows.Forms.Button();
            this.imageListLog = new System.Windows.Forms.ImageList(this.components);
            this.buttonDown = new System.Windows.Forms.Button();
            this.panelVert = new System.Windows.Forms.Panel();
            this.buttonPause = new System.Windows.Forms.Button();
            this.buttonPlay = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.buttonClipboard = new System.Windows.Forms.Button();
            this.logControl = new Automatak.DNP3.Simulator.LogControl();
            this.panelVert.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonUp
            // 
            this.buttonUp.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonUp.ImageIndex = 0;
            this.buttonUp.ImageList = this.imageListLog;
            this.buttonUp.Location = new System.Drawing.Point(3, 3);
            this.buttonUp.Name = "buttonUp";
            this.buttonUp.Size = new System.Drawing.Size(37, 37);
            this.buttonUp.TabIndex = 1;
            this.buttonUp.UseVisualStyleBackColor = true;
            // 
            // imageListLog
            // 
            this.imageListLog.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageListLog.ImageStream")));
            this.imageListLog.TransparentColor = System.Drawing.Color.Transparent;
            this.imageListLog.Images.SetKeyName(0, "arrow_up.ico");
            this.imageListLog.Images.SetKeyName(1, "arrow_down.ico");
            this.imageListLog.Images.SetKeyName(2, "play.ico");
            this.imageListLog.Images.SetKeyName(3, "pause.ico");
            this.imageListLog.Images.SetKeyName(4, "clipboard_diskette.ico");
            // 
            // buttonDown
            // 
            this.buttonDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonDown.ImageIndex = 1;
            this.buttonDown.ImageList = this.imageListLog;
            this.buttonDown.Location = new System.Drawing.Point(3, 380);
            this.buttonDown.Name = "buttonDown";
            this.buttonDown.Size = new System.Drawing.Size(37, 37);
            this.buttonDown.TabIndex = 2;
            this.buttonDown.UseVisualStyleBackColor = true;
            // 
            // panelVert
            // 
            this.panelVert.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelVert.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelVert.Controls.Add(this.buttonClipboard);
            this.panelVert.Controls.Add(this.buttonPause);
            this.panelVert.Controls.Add(this.buttonPlay);
            this.panelVert.Controls.Add(this.buttonDown);
            this.panelVert.Controls.Add(this.buttonUp);
            this.panelVert.Location = new System.Drawing.Point(921, 0);
            this.panelVert.Name = "panelVert";
            this.panelVert.Size = new System.Drawing.Size(45, 423);
            this.panelVert.TabIndex = 4;
            // 
            // buttonPause
            // 
            this.buttonPause.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPause.ImageIndex = 3;
            this.buttonPause.ImageList = this.imageListLog;
            this.buttonPause.Location = new System.Drawing.Point(3, 339);
            this.buttonPause.Name = "buttonPause";
            this.buttonPause.Size = new System.Drawing.Size(37, 37);
            this.buttonPause.TabIndex = 4;
            this.buttonPause.UseVisualStyleBackColor = true;
            this.buttonPause.Click += new System.EventHandler(this.buttonPause_Click);
            // 
            // buttonPlay
            // 
            this.buttonPlay.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPlay.Enabled = false;
            this.buttonPlay.ImageIndex = 2;
            this.buttonPlay.ImageList = this.imageListLog;
            this.buttonPlay.Location = new System.Drawing.Point(3, 298);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(37, 37);
            this.buttonPlay.TabIndex = 3;
            this.buttonPlay.UseVisualStyleBackColor = true;
            this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 422);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(969, 26);
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
            // buttonClipboard
            // 
            this.buttonClipboard.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClipboard.ImageIndex = 4;
            this.buttonClipboard.ImageList = this.imageListLog;
            this.buttonClipboard.Location = new System.Drawing.Point(3, 257);
            this.buttonClipboard.Name = "buttonClipboard";
            this.buttonClipboard.Size = new System.Drawing.Size(37, 37);
            this.buttonClipboard.TabIndex = 5;
            this.buttonClipboard.UseVisualStyleBackColor = true;
            this.buttonClipboard.Click += new System.EventHandler(this.buttonClipboard_Click);
            // 
            // logControl
            // 
            this.logControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.logControl.AutoScroll = true;
            this.logControl.AutoScrollMinSize = new System.Drawing.Size(100, 100);
            this.logControl.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.logControl.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.logControl.Font = new System.Drawing.Font("Courier New", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.logControl.Location = new System.Drawing.Point(0, 0);
            this.logControl.Margin = new System.Windows.Forms.Padding(0);
            this.logControl.MinimumSize = new System.Drawing.Size(500, 150);
            this.logControl.Name = "logControl";
            this.logControl.Size = new System.Drawing.Size(918, 423);
            this.logControl.SpacingFactor = 0.1F;
            this.logControl.TabIndex = 0;
            // 
            // LogWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.panelVert);
            this.Controls.Add(this.logControl);
            this.Name = "LogWindow";
            this.Size = new System.Drawing.Size(969, 448);
            this.panelVert.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private LogControl logControl;
        private System.Windows.Forms.Button buttonUp;
        private System.Windows.Forms.ImageList imageListLog;
        private System.Windows.Forms.Button buttonDown;
        private System.Windows.Forms.Panel panelVert;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.Button buttonPlay;
        private System.Windows.Forms.Button buttonPause;
        private System.Windows.Forms.Button buttonClipboard;
    }
}
