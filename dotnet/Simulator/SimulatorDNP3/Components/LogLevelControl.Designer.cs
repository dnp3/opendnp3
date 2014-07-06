namespace Automatak.Simulator.DNP3.Components
{
    partial class LogLevelControl
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
            this.checkBoxLinkRaw = new System.Windows.Forms.CheckBox();
            this.checkBoxLink = new System.Windows.Forms.CheckBox();
            this.checkBoxAppHeader = new System.Windows.Forms.CheckBox();
            this.checkBoxTransport = new System.Windows.Forms.CheckBox();
            this.checkBoxInfo = new System.Windows.Forms.CheckBox();
            this.checkBoxWarn = new System.Windows.Forms.CheckBox();
            this.checkBoxError = new System.Windows.Forms.CheckBox();
            this.checkBoxDebug = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // checkBoxLinkRaw
            // 
            this.checkBoxLinkRaw.AutoSize = true;
            this.checkBoxLinkRaw.Checked = true;
            this.checkBoxLinkRaw.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLinkRaw.Location = new System.Drawing.Point(85, 35);
            this.checkBoxLinkRaw.Name = "checkBoxLinkRaw";
            this.checkBoxLinkRaw.Size = new System.Drawing.Size(68, 17);
            this.checkBoxLinkRaw.TabIndex = 23;
            this.checkBoxLinkRaw.Text = "Link Hex";
            this.checkBoxLinkRaw.UseVisualStyleBackColor = true;
            // 
            // checkBoxLink
            // 
            this.checkBoxLink.AutoSize = true;
            this.checkBoxLink.Checked = true;
            this.checkBoxLink.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLink.Location = new System.Drawing.Point(85, 12);
            this.checkBoxLink.Name = "checkBoxLink";
            this.checkBoxLink.Size = new System.Drawing.Size(46, 17);
            this.checkBoxLink.TabIndex = 22;
            this.checkBoxLink.Text = "Link";
            this.checkBoxLink.UseVisualStyleBackColor = true;
            // 
            // checkBoxAppHeader
            // 
            this.checkBoxAppHeader.AutoSize = true;
            this.checkBoxAppHeader.Checked = true;
            this.checkBoxAppHeader.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAppHeader.Location = new System.Drawing.Point(85, 81);
            this.checkBoxAppHeader.Name = "checkBoxAppHeader";
            this.checkBoxAppHeader.Size = new System.Drawing.Size(78, 17);
            this.checkBoxAppHeader.TabIndex = 25;
            this.checkBoxAppHeader.Text = "Application";
            this.checkBoxAppHeader.UseVisualStyleBackColor = true;
            // 
            // checkBoxTransport
            // 
            this.checkBoxTransport.AutoSize = true;
            this.checkBoxTransport.Checked = true;
            this.checkBoxTransport.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxTransport.Location = new System.Drawing.Point(85, 58);
            this.checkBoxTransport.Name = "checkBoxTransport";
            this.checkBoxTransport.Size = new System.Drawing.Size(71, 17);
            this.checkBoxTransport.TabIndex = 24;
            this.checkBoxTransport.Text = "Transport";
            this.checkBoxTransport.UseVisualStyleBackColor = true;
            // 
            // checkBoxInfo
            // 
            this.checkBoxInfo.AutoSize = true;
            this.checkBoxInfo.Checked = true;
            this.checkBoxInfo.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxInfo.Location = new System.Drawing.Point(13, 58);
            this.checkBoxInfo.Name = "checkBoxInfo";
            this.checkBoxInfo.Size = new System.Drawing.Size(44, 17);
            this.checkBoxInfo.TabIndex = 28;
            this.checkBoxInfo.Text = "Info";
            this.checkBoxInfo.UseVisualStyleBackColor = true;
            // 
            // checkBoxWarn
            // 
            this.checkBoxWarn.AutoSize = true;
            this.checkBoxWarn.Checked = true;
            this.checkBoxWarn.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxWarn.Location = new System.Drawing.Point(13, 35);
            this.checkBoxWarn.Name = "checkBoxWarn";
            this.checkBoxWarn.Size = new System.Drawing.Size(66, 17);
            this.checkBoxWarn.TabIndex = 26;
            this.checkBoxWarn.Text = "Warning";
            this.checkBoxWarn.UseVisualStyleBackColor = true;
            // 
            // checkBoxError
            // 
            this.checkBoxError.AutoSize = true;
            this.checkBoxError.Checked = true;
            this.checkBoxError.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxError.Location = new System.Drawing.Point(13, 12);
            this.checkBoxError.Name = "checkBoxError";
            this.checkBoxError.Size = new System.Drawing.Size(48, 17);
            this.checkBoxError.TabIndex = 27;
            this.checkBoxError.Text = "Error";
            this.checkBoxError.UseVisualStyleBackColor = true;
            // 
            // checkBoxDebug
            // 
            this.checkBoxDebug.AutoSize = true;
            this.checkBoxDebug.Location = new System.Drawing.Point(13, 81);
            this.checkBoxDebug.Name = "checkBoxDebug";
            this.checkBoxDebug.Size = new System.Drawing.Size(58, 17);
            this.checkBoxDebug.TabIndex = 29;
            this.checkBoxDebug.Text = "Debug";
            this.checkBoxDebug.UseVisualStyleBackColor = true;
            // 
            // LogLevelControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.checkBoxInfo);
            this.Controls.Add(this.checkBoxWarn);
            this.Controls.Add(this.checkBoxError);
            this.Controls.Add(this.checkBoxDebug);
            this.Controls.Add(this.checkBoxLinkRaw);
            this.Controls.Add(this.checkBoxLink);
            this.Controls.Add(this.checkBoxAppHeader);
            this.Controls.Add(this.checkBoxTransport);
            this.Name = "LogLevelControl";
            this.Size = new System.Drawing.Size(168, 115);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox checkBoxLinkRaw;
        private System.Windows.Forms.CheckBox checkBoxLink;
        private System.Windows.Forms.CheckBox checkBoxAppHeader;
        private System.Windows.Forms.CheckBox checkBoxTransport;
        private System.Windows.Forms.CheckBox checkBoxInfo;
        private System.Windows.Forms.CheckBox checkBoxWarn;
        private System.Windows.Forms.CheckBox checkBoxError;
        private System.Windows.Forms.CheckBox checkBoxDebug;

    }
}
