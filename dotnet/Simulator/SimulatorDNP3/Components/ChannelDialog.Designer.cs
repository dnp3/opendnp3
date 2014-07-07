namespace Automatak.Simulator.DNP3.Components
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
            this.tabPageServer = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxID = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownMaxRetryMS = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownMinRetryMS = new System.Windows.Forms.NumericUpDown();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.label11 = new System.Windows.Forms.Label();
            this.comboBoxFlowControl = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.comboBoxParity = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.comboBoxStopBits = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.numericUpDownDataBits = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownBaud = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.comboBoxSerialDeviceName = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.numericUpDownServerPort = new System.Windows.Forms.NumericUpDown();
            this.label13 = new System.Windows.Forms.Label();
            this.textBoxServerHost = new System.Windows.Forms.TextBox();
            this.logLevelControl1 = new Automatak.Simulator.DNP3.Components.LogLevelControl();
            this.groupBoxButtons.SuspendLayout();
            this.tabControlChannelType.SuspendLayout();
            this.tabPageTCPClient.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPort)).BeginInit();
            this.tabPageServer.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMaxRetryMS)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMinRetryMS)).BeginInit();
            this.statusStrip.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownDataBits)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownBaud)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownServerPort)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBoxButtons
            // 
            this.groupBoxButtons.Controls.Add(this.buttonCancel);
            this.groupBoxButtons.Controls.Add(this.buttonADD);
            this.groupBoxButtons.Location = new System.Drawing.Point(14, 339);
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
            this.tabControlChannelType.Controls.Add(this.tabPageServer);
            this.tabControlChannelType.Controls.Add(this.tabPage1);
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
            this.label5.Location = new System.Drawing.Point(141, 45);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(26, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Port";
            // 
            // numericUpDownPort
            // 
            this.numericUpDownPort.Location = new System.Drawing.Point(15, 43);
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
            this.label4.Location = new System.Drawing.Point(141, 19);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Host";
            // 
            // textBoxHost
            // 
            this.textBoxHost.Location = new System.Drawing.Point(15, 16);
            this.textBoxHost.Name = "textBoxHost";
            this.textBoxHost.Size = new System.Drawing.Size(120, 20);
            this.textBoxHost.TabIndex = 0;
            this.textBoxHost.Text = "127.0.0.1";
            // 
            // tabPageServer
            // 
            this.tabPageServer.Controls.Add(this.label12);
            this.tabPageServer.Controls.Add(this.numericUpDownServerPort);
            this.tabPageServer.Controls.Add(this.label13);
            this.tabPageServer.Controls.Add(this.textBoxServerHost);
            this.tabPageServer.Location = new System.Drawing.Point(4, 22);
            this.tabPageServer.Name = "tabPageServer";
            this.tabPageServer.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageServer.Size = new System.Drawing.Size(442, 158);
            this.tabPageServer.TabIndex = 1;
            this.tabPageServer.Text = "TCP Server";
            this.tabPageServer.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBoxID);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.numericUpDownMaxRetryMS);
            this.groupBox1.Controls.Add(this.numericUpDownMinRetryMS);
            this.groupBox1.Location = new System.Drawing.Point(12, 202);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(216, 131);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Settings";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(110, 33);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Alias";
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
            // numericUpDownMaxRetryMS
            // 
            this.numericUpDownMaxRetryMS.Increment = new decimal(new int[] {
            250,
            0,
            0,
            0});
            this.numericUpDownMaxRetryMS.Location = new System.Drawing.Point(16, 90);
            this.numericUpDownMaxRetryMS.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDownMaxRetryMS.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.numericUpDownMaxRetryMS.Name = "numericUpDownMaxRetryMS";
            this.numericUpDownMaxRetryMS.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownMaxRetryMS.TabIndex = 1;
            this.numericUpDownMaxRetryMS.Value = new decimal(new int[] {
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
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip.Location = new System.Drawing.Point(0, 426);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(471, 22);
            this.statusStrip.SizingGrip = false;
            this.statusStrip.TabIndex = 5;
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            this.toolStripStatusLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.logLevelControl1);
            this.groupBox2.Location = new System.Drawing.Point(234, 202);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(230, 131);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Logging";
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label11);
            this.tabPage1.Controls.Add(this.comboBoxFlowControl);
            this.tabPage1.Controls.Add(this.label10);
            this.tabPage1.Controls.Add(this.comboBoxParity);
            this.tabPage1.Controls.Add(this.label9);
            this.tabPage1.Controls.Add(this.comboBoxStopBits);
            this.tabPage1.Controls.Add(this.label8);
            this.tabPage1.Controls.Add(this.numericUpDownDataBits);
            this.tabPage1.Controls.Add(this.numericUpDownBaud);
            this.tabPage1.Controls.Add(this.label7);
            this.tabPage1.Controls.Add(this.label6);
            this.tabPage1.Controls.Add(this.comboBoxSerialDeviceName);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(442, 158);
            this.tabPage1.TabIndex = 2;
            this.tabPage1.Text = "Serial";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(352, 129);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(65, 13);
            this.label11.TabIndex = 24;
            this.label11.Text = "Flow Control";
            // 
            // comboBoxFlowControl
            // 
            this.comboBoxFlowControl.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFlowControl.FormattingEnabled = true;
            this.comboBoxFlowControl.Location = new System.Drawing.Point(249, 125);
            this.comboBoxFlowControl.Name = "comboBoxFlowControl";
            this.comboBoxFlowControl.Size = new System.Drawing.Size(96, 21);
            this.comboBoxFlowControl.TabIndex = 23;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(351, 73);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(33, 13);
            this.label10.TabIndex = 22;
            this.label10.Text = "Parity";
            // 
            // comboBoxParity
            // 
            this.comboBoxParity.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxParity.FormattingEnabled = true;
            this.comboBoxParity.Location = new System.Drawing.Point(249, 69);
            this.comboBoxParity.Name = "comboBoxParity";
            this.comboBoxParity.Size = new System.Drawing.Size(96, 21);
            this.comboBoxParity.TabIndex = 21;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(351, 100);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(49, 13);
            this.label9.TabIndex = 20;
            this.label9.Text = "Stop Bits";
            // 
            // comboBoxStopBits
            // 
            this.comboBoxStopBits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxStopBits.FormattingEnabled = true;
            this.comboBoxStopBits.Location = new System.Drawing.Point(249, 96);
            this.comboBoxStopBits.Name = "comboBoxStopBits";
            this.comboBoxStopBits.Size = new System.Drawing.Size(96, 21);
            this.comboBoxStopBits.TabIndex = 19;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(351, 41);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(50, 13);
            this.label8.TabIndex = 18;
            this.label8.Text = "Data Bits";
            // 
            // numericUpDownDataBits
            // 
            this.numericUpDownDataBits.Location = new System.Drawing.Point(249, 39);
            this.numericUpDownDataBits.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.numericUpDownDataBits.Minimum = new decimal(new int[] {
            6,
            0,
            0,
            0});
            this.numericUpDownDataBits.Name = "numericUpDownDataBits";
            this.numericUpDownDataBits.Size = new System.Drawing.Size(96, 20);
            this.numericUpDownDataBits.TabIndex = 17;
            this.numericUpDownDataBits.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            // 
            // numericUpDownBaud
            // 
            this.numericUpDownBaud.Increment = new decimal(new int[] {
            1200,
            0,
            0,
            0});
            this.numericUpDownBaud.Location = new System.Drawing.Point(249, 13);
            this.numericUpDownBaud.Maximum = new decimal(new int[] {
            115200,
            0,
            0,
            0});
            this.numericUpDownBaud.Minimum = new decimal(new int[] {
            1200,
            0,
            0,
            0});
            this.numericUpDownBaud.Name = "numericUpDownBaud";
            this.numericUpDownBaud.Size = new System.Drawing.Size(96, 20);
            this.numericUpDownBaud.TabIndex = 16;
            this.numericUpDownBaud.Value = new decimal(new int[] {
            9600,
            0,
            0,
            0});
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(351, 16);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(58, 13);
            this.label7.TabIndex = 15;
            this.label7.Text = "Baud Rate";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(161, 16);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(26, 13);
            this.label6.TabIndex = 14;
            this.label6.Text = "Port";
            // 
            // comboBoxSerialDeviceName
            // 
            this.comboBoxSerialDeviceName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxSerialDeviceName.FormattingEnabled = true;
            this.comboBoxSerialDeviceName.Location = new System.Drawing.Point(26, 13);
            this.comboBoxSerialDeviceName.Name = "comboBoxSerialDeviceName";
            this.comboBoxSerialDeviceName.Size = new System.Drawing.Size(129, 21);
            this.comboBoxSerialDeviceName.TabIndex = 13;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(139, 46);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(26, 13);
            this.label12.TabIndex = 12;
            this.label12.Text = "Port";
            // 
            // numericUpDownServerPort
            // 
            this.numericUpDownServerPort.Location = new System.Drawing.Point(13, 44);
            this.numericUpDownServerPort.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownServerPort.Name = "numericUpDownServerPort";
            this.numericUpDownServerPort.Size = new System.Drawing.Size(120, 20);
            this.numericUpDownServerPort.TabIndex = 11;
            this.numericUpDownServerPort.Value = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(139, 20);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(84, 13);
            this.label13.TabIndex = 10;
            this.label13.Text = "Bind To Adapter";
            // 
            // textBoxServerHost
            // 
            this.textBoxServerHost.Location = new System.Drawing.Point(13, 17);
            this.textBoxServerHost.Name = "textBoxServerHost";
            this.textBoxServerHost.Size = new System.Drawing.Size(120, 20);
            this.textBoxServerHost.TabIndex = 9;
            this.textBoxServerHost.Text = "127.0.0.1";
            // 
            // logLevelControl1
            // 
            this.logLevelControl1.Location = new System.Drawing.Point(47, 19);
            this.logLevelControl1.Name = "logLevelControl1";
            this.logLevelControl1.Size = new System.Drawing.Size(168, 115);
            this.logLevelControl1.TabIndex = 0;
            // 
            // ChannelDialog
            // 
            this.AcceptButton = this.buttonADD;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.buttonCancel;
            this.ClientSize = new System.Drawing.Size(471, 448);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.statusStrip);
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
            this.Text = "Add DNP3 Channel";
            this.TopMost = true;
            this.groupBoxButtons.ResumeLayout(false);
            this.tabControlChannelType.ResumeLayout(false);
            this.tabPageTCPClient.ResumeLayout(false);
            this.tabPageTCPClient.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPort)).EndInit();
            this.tabPageServer.ResumeLayout(false);
            this.tabPageServer.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMaxRetryMS)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownMinRetryMS)).EndInit();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownDataBits)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownBaud)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownServerPort)).EndInit();
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
        private System.Windows.Forms.NumericUpDown numericUpDownMaxRetryMS;
        private System.Windows.Forms.TextBox textBoxID;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.TextBox textBoxHost;
        private System.Windows.Forms.NumericUpDown numericUpDownPort;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TabPage tabPageServer;
        private System.Windows.Forms.GroupBox groupBox2;
        private LogLevelControl logLevelControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox comboBoxFlowControl;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox comboBoxParity;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboBoxStopBits;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.NumericUpDown numericUpDownDataBits;
        private System.Windows.Forms.NumericUpDown numericUpDownBaud;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox comboBoxSerialDeviceName;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.NumericUpDown numericUpDownServerPort;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBoxServerHost;
    }
}