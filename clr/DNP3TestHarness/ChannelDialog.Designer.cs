namespace Automatak.DNP3.Simulator
{
    partial class ChannelDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ChannelDialog));
            this.groupBoxButtons = new System.Windows.Forms.GroupBox();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonADD = new System.Windows.Forms.Button();
            this.tabControlChannelType = new System.Windows.Forms.TabControl();
            this.tabPageTCPClient = new System.Windows.Forms.TabPage();
            this.label5 = new System.Windows.Forms.Label();
            this.numericUpDownPort = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxHost = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxID = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownMaxRetry = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownMinRetryMS = new System.Windows.Forms.NumericUpDown();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.groupBoxLogging = new System.Windows.Forms.GroupBox();
            this.checkBoxAppObject = new System.Windows.Forms.CheckBox();
            this.checkBoxAppHeader = new System.Windows.Forms.CheckBox();
            this.checkBoxTransport = new System.Windows.Forms.CheckBox();
            this.checkBoxLinkRaw = new System.Windows.Forms.CheckBox();
            this.checkBoxLink = new System.Windows.Forms.CheckBox();
            this.checkBoxDebug = new System.Windows.Forms.CheckBox();
            this.checkBoxInfo = new System.Windows.Forms.CheckBox();
            this.checkBoxError = new System.Windows.Forms.CheckBox();
            this.checkBoxWarn = new System.Windows.Forms.CheckBox();
            this.tabPageSerial = new System.Windows.Forms.TabPage();
            this.comboBoxSerialDeviceName = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBoxButtons.SuspendLayout();
            this.tabControlChannelType.SuspendLayout();
            this.tabPageTCPClient.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPort)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMaxRetry)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMinRetryMS)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.groupBoxLogging.SuspendLayout();
            this.tabPageSerial.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxButtons
            // 
            this.groupBoxButtons.Controls.Add(this.buttonCancel);
            this.groupBoxButtons.Controls.Add(this.buttonADD);
            this.groupBoxButtons.Location = new System.Drawing.Point(14, 356);
            this.groupBoxButtons.Name = "groupBoxButtons";
            this.groupBoxButtons.Size = new System.Drawing.Size(450, 78);
            this.groupBoxButtons.TabIndex = 0;
            this.groupBoxButtons.TabStop = false;
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonCancel.Location = new System.Drawing.Point(319, 19);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(116, 42);
            this.buttonCancel.TabIndex = 1;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            // 
            // buttonADD
            // 
            this.buttonADD.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonADD.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonADD.Location = new System.Drawing.Point(197, 19);
            this.buttonADD.Name = "buttonADD";
            this.buttonADD.Size = new System.Drawing.Size(116, 42);
            this.buttonADD.TabIndex = 0;
            this.buttonADD.Text = "Add";
            this.buttonADD.UseVisualStyleBackColor = true;
            this.buttonADD.Click += new System.EventHandler(this.buttonADD_Click);
            // 
            // tabControlChannelType
            // 
            this.tabControlChannelType.Controls.Add(this.tabPageTCPClient);
            this.tabControlChannelType.Controls.Add(this.tabPageSerial);
            this.tabControlChannelType.Location = new System.Drawing.Point(14, 12);
            this.tabControlChannelType.Name = "tabControlChannelType";
            this.tabControlChannelType.SelectedIndex = 0;
            this.tabControlChannelType.Size = new System.Drawing.Size(450, 184);
            this.tabControlChannelType.TabIndex = 2;
            // 
            // tabPageTCPClient
            // 
            this.tabPageTCPClient.Controls.Add(this.label5);
            this.tabPageTCPClient.Controls.Add(this.numericUpDownPort);
            this.tabPageTCPClient.Controls.Add(this.label4);
            this.tabPageTCPClient.Controls.Add(this.textBoxHost);
            this.tabPageTCPClient.Location = new System.Drawing.Point(4, 22);
            this.tabPageTCPClient.Name = "tabPageTCPClient";
            this.tabPageTCPClient.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTCPClient.Size = new System.Drawing.Size(442, 158);
            this.tabPageTCPClient.TabIndex = 0;
            this.tabPageTCPClient.Text = "TCP Client";
            this.tabPageTCPClient.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(146, 48);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(26, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Port";
            // 
            // numericUpDownPort
            // 
            this.numericUpDownPort.Location = new System.Drawing.Point(20, 46);
            this.numericUpDownPort.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownPort.Name = "numericUpDownPort";
            this.numericUpDownPort.Size = new System.Drawing.Size(120, 20);
            this.numericUpDownPort.TabIndex = 7;
            this.numericUpDownPort.Value = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(146, 22);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Host";
            // 
            // textBoxHost
            // 
            this.textBoxHost.Location = new System.Drawing.Point(20, 19);
            this.textBoxHost.Name = "textBoxHost";
            this.textBoxHost.Size = new System.Drawing.Size(120, 20);
            this.textBoxHost.TabIndex = 0;
            this.textBoxHost.Text = "127.0.0.1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBoxID);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.numericUpDownMaxRetry);
            this.groupBox1.Controls.Add(this.numericUpDownMinRetryMS);
            this.groupBox1.Location = new System.Drawing.Point(12, 202);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(194, 148);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Settings";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(110, 33);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(54, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Logger ID";
            // 
            // textBoxID
            // 
            this.textBoxID.Location = new System.Drawing.Point(16, 28);
            this.textBoxID.Name = "textBoxID";
            this.textBoxID.Size = new System.Drawing.Size(88, 20);
            this.textBoxID.TabIndex = 4;
            this.textBoxID.Text = "channel";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(110, 93);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Max Retry (ms)";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(110, 61);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(74, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Min Retry (ms)";
            // 
            // numericUpDownMaxRetry
            // 
            this.numericUpDownMaxRetry.Increment = new decimal(new int[] {
            250,
            0,
            0,
            0});
            this.numericUpDownMaxRetry.Location = new System.Drawing.Point(16, 90);
            this.numericUpDownMaxRetry.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDownMaxRetry.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numericUpDownMaxRetry.Name = "numericUpDownMaxRetry";
            this.numericUpDownMaxRetry.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownMaxRetry.TabIndex = 1;
            this.numericUpDownMaxRetry.Value = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            // 
            // numericUpDownMinRetryMS
            // 
            this.numericUpDownMinRetryMS.Increment = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numericUpDownMinRetryMS.Location = new System.Drawing.Point(16, 59);
            this.numericUpDownMinRetryMS.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDownMinRetryMS.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numericUpDownMinRetryMS.Name = "numericUpDownMinRetryMS";
            this.numericUpDownMinRetryMS.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownMinRetryMS.TabIndex = 0;
            this.numericUpDownMinRetryMS.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 443);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(478, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 5;
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            this.toolStripStatusLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // groupBoxLogging
            // 
            this.groupBoxLogging.Controls.Add(this.checkBoxAppObject);
            this.groupBoxLogging.Controls.Add(this.checkBoxAppHeader);
            this.groupBoxLogging.Controls.Add(this.checkBoxTransport);
            this.groupBoxLogging.Controls.Add(this.checkBoxLinkRaw);
            this.groupBoxLogging.Controls.Add(this.checkBoxLink);
            this.groupBoxLogging.Controls.Add(this.checkBoxDebug);
            this.groupBoxLogging.Controls.Add(this.checkBoxInfo);
            this.groupBoxLogging.Controls.Add(this.checkBoxError);
            this.groupBoxLogging.Controls.Add(this.checkBoxWarn);
            this.groupBoxLogging.Location = new System.Drawing.Point(212, 202);
            this.groupBoxLogging.Name = "groupBoxLogging";
            this.groupBoxLogging.Size = new System.Drawing.Size(248, 148);
            this.groupBoxLogging.TabIndex = 6;
            this.groupBoxLogging.TabStop = false;
            this.groupBoxLogging.Text = "Logging";
            // 
            // checkBoxAppObject
            // 
            this.checkBoxAppObject.AutoSize = true;
            this.checkBoxAppObject.Checked = true;
            this.checkBoxAppObject.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAppObject.Location = new System.Drawing.Point(104, 117);
            this.checkBoxAppObject.Name = "checkBoxAppObject";
            this.checkBoxAppObject.Size = new System.Drawing.Size(122, 17);
            this.checkBoxAppObject.TabIndex = 11;
            this.checkBoxAppObject.Text = "App Object Headers";
            this.checkBoxAppObject.UseVisualStyleBackColor = true;
            // 
            // checkBoxAppHeader
            // 
            this.checkBoxAppHeader.AutoSize = true;
            this.checkBoxAppHeader.Checked = true;
            this.checkBoxAppHeader.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAppHeader.Location = new System.Drawing.Point(104, 94);
            this.checkBoxAppHeader.Name = "checkBoxAppHeader";
            this.checkBoxAppHeader.Size = new System.Drawing.Size(83, 17);
            this.checkBoxAppHeader.TabIndex = 10;
            this.checkBoxAppHeader.Text = "App Header";
            this.checkBoxAppHeader.UseVisualStyleBackColor = true;
            // 
            // checkBoxTransport
            // 
            this.checkBoxTransport.AutoSize = true;
            this.checkBoxTransport.Checked = true;
            this.checkBoxTransport.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxTransport.Location = new System.Drawing.Point(104, 71);
            this.checkBoxTransport.Name = "checkBoxTransport";
            this.checkBoxTransport.Size = new System.Drawing.Size(71, 17);
            this.checkBoxTransport.TabIndex = 9;
            this.checkBoxTransport.Text = "Transport";
            this.checkBoxTransport.UseVisualStyleBackColor = true;
            // 
            // checkBoxLinkRaw
            // 
            this.checkBoxLinkRaw.AutoSize = true;
            this.checkBoxLinkRaw.Checked = true;
            this.checkBoxLinkRaw.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLinkRaw.Location = new System.Drawing.Point(104, 48);
            this.checkBoxLinkRaw.Name = "checkBoxLinkRaw";
            this.checkBoxLinkRaw.Size = new System.Drawing.Size(68, 17);
            this.checkBoxLinkRaw.TabIndex = 8;
            this.checkBoxLinkRaw.Text = "LinkRaw";
            this.checkBoxLinkRaw.UseVisualStyleBackColor = true;
            // 
            // checkBoxLink
            // 
            this.checkBoxLink.AutoSize = true;
            this.checkBoxLink.Checked = true;
            this.checkBoxLink.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLink.Location = new System.Drawing.Point(104, 25);
            this.checkBoxLink.Name = "checkBoxLink";
            this.checkBoxLink.Size = new System.Drawing.Size(46, 17);
            this.checkBoxLink.TabIndex = 7;
            this.checkBoxLink.Text = "Link";
            this.checkBoxLink.UseVisualStyleBackColor = true;
            // 
            // checkBoxDebug
            // 
            this.checkBoxDebug.AutoSize = true;
            this.checkBoxDebug.Location = new System.Drawing.Point(22, 94);
            this.checkBoxDebug.Name = "checkBoxDebug";
            this.checkBoxDebug.Size = new System.Drawing.Size(58, 17);
            this.checkBoxDebug.TabIndex = 3;
            this.checkBoxDebug.Text = "Debug";
            this.checkBoxDebug.UseVisualStyleBackColor = true;
            // 
            // checkBoxInfo
            // 
            this.checkBoxInfo.AutoSize = true;
            this.checkBoxInfo.Checked = true;
            this.checkBoxInfo.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxInfo.Location = new System.Drawing.Point(22, 71);
            this.checkBoxInfo.Name = "checkBoxInfo";
            this.checkBoxInfo.Size = new System.Drawing.Size(44, 17);
            this.checkBoxInfo.TabIndex = 2;
            this.checkBoxInfo.Text = "Info";
            this.checkBoxInfo.UseVisualStyleBackColor = true;
            // 
            // checkBoxError
            // 
            this.checkBoxError.AutoSize = true;
            this.checkBoxError.Checked = true;
            this.checkBoxError.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxError.Location = new System.Drawing.Point(22, 25);
            this.checkBoxError.Name = "checkBoxError";
            this.checkBoxError.Size = new System.Drawing.Size(48, 17);
            this.checkBoxError.TabIndex = 1;
            this.checkBoxError.Text = "Error";
            this.checkBoxError.UseVisualStyleBackColor = true;
            // 
            // checkBoxWarn
            // 
            this.checkBoxWarn.AutoSize = true;
            this.checkBoxWarn.Checked = true;
            this.checkBoxWarn.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxWarn.Location = new System.Drawing.Point(22, 48);
            this.checkBoxWarn.Name = "checkBoxWarn";
            this.checkBoxWarn.Size = new System.Drawing.Size(66, 17);
            this.checkBoxWarn.TabIndex = 0;
            this.checkBoxWarn.Text = "Warning";
            this.checkBoxWarn.UseVisualStyleBackColor = true;
            // 
            // tabPageSerial
            // 
            this.tabPageSerial.Controls.Add(this.label6);
            this.tabPageSerial.Controls.Add(this.comboBoxSerialDeviceName);
            this.tabPageSerial.Location = new System.Drawing.Point(4, 22);
            this.tabPageSerial.Name = "tabPageSerial";
            this.tabPageSerial.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageSerial.Size = new System.Drawing.Size(442, 158);
            this.tabPageSerial.TabIndex = 1;
            this.tabPageSerial.Text = "Serial";
            this.tabPageSerial.UseVisualStyleBackColor = true;
            // 
            // comboBoxSerialDeviceName
            // 
            this.comboBoxSerialDeviceName.FormattingEnabled = true;
            this.comboBoxSerialDeviceName.Location = new System.Drawing.Point(10, 15);
            this.comboBoxSerialDeviceName.Name = "comboBoxSerialDeviceName";
            this.comboBoxSerialDeviceName.Size = new System.Drawing.Size(148, 21);
            this.comboBoxSerialDeviceName.TabIndex = 0;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(164, 18);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(72, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Device Name";
            // 
            // ChannelDialog
            // 
            this.AcceptButton = this.buttonADD;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(478, 465);
            this.Controls.Add(this.groupBoxLogging);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabControlChannelType);
            this.Controls.Add(this.groupBoxButtons);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ChannelDialog";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Add Channel";
            this.TopMost = true;
            this.groupBoxButtons.ResumeLayout(false);
            this.tabControlChannelType.ResumeLayout(false);
            this.tabPageTCPClient.ResumeLayout(false);
            this.tabPageTCPClient.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPort)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMaxRetry)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMinRetryMS)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.groupBoxLogging.ResumeLayout(false);
            this.groupBoxLogging.PerformLayout();
            this.tabPageSerial.ResumeLayout(false);
            this.tabPageSerial.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxButtons;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.Button buttonADD;
        private System.Windows.Forms.TabControl tabControlChannelType;
        private System.Windows.Forms.TabPage tabPageTCPClient;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown numericUpDownMinRetryMS;
        private System.Windows.Forms.NumericUpDown numericUpDownMaxRetry;
        private System.Windows.Forms.TextBox textBoxID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.TextBox textBoxHost;
        private System.Windows.Forms.NumericUpDown numericUpDownPort;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBoxLogging;
        private System.Windows.Forms.CheckBox checkBoxDebug;
        private System.Windows.Forms.CheckBox checkBoxInfo;
        private System.Windows.Forms.CheckBox checkBoxError;
        private System.Windows.Forms.CheckBox checkBoxWarn;
        private System.Windows.Forms.CheckBox checkBoxLink;
        private System.Windows.Forms.CheckBox checkBoxLinkRaw;
        private System.Windows.Forms.CheckBox checkBoxTransport;
        private System.Windows.Forms.CheckBox checkBoxAppHeader;
        private System.Windows.Forms.CheckBox checkBoxAppObject;
        private System.Windows.Forms.TabPage tabPageSerial;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox comboBoxSerialDeviceName;
    }
}