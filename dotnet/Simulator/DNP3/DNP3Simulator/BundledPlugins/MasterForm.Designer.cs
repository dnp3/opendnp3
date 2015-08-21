namespace Automatak.Simulator.DNP3
{
    partial class MasterForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MasterForm));
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.masterScanControl1 = new Automatak.Simulator.DNP3.Components.MasterScanControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.crobControl = new Automatak.Simulator.DNP3.CROBControl();
            this.buttonSelectOperate = new System.Windows.Forms.Button();
            this.buttonDirectOperate = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.analogOutputControl = new Automatak.Simulator.DNP3.Components.AnalogOutputControl();
            this.buttonSBOAO = new System.Windows.Forms.Button();
            this.buttonDOAO = new System.Windows.Forms.Button();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.comboBoxFunctionCode = new System.Windows.Forms.ComboBox();
            this.buttonSendFunction = new System.Windows.Forms.Button();
            this.comboBoxTypes = new System.Windows.Forms.ComboBox();
            this.measurementView = new Automatak.Simulator.DNP3.Commons.MeasurementView();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.tabControl1);
            this.splitContainer1.Panel1.Controls.Add(this.comboBoxTypes);
            this.splitContainer1.Panel1.Padding = new System.Windows.Forms.Padding(5);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.measurementView);
            this.splitContainer1.Size = new System.Drawing.Size(1048, 519);
            this.splitContainer1.SplitterDistance = 296;
            this.splitContainer1.TabIndex = 1;
            // 
            // tabControl1
            // 
            this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Left;
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Location = new System.Drawing.Point(5, 32);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(286, 478);
            this.tabControl1.TabIndex = 3;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.masterScanControl1);
            this.tabPage1.Location = new System.Drawing.Point(23, 4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(259, 470);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Scans";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // masterScanControl1
            // 
            this.masterScanControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.masterScanControl1.Location = new System.Drawing.Point(3, 3);
            this.masterScanControl1.Name = "masterScanControl1";
            this.masterScanControl1.Size = new System.Drawing.Size(253, 464);
            this.masterScanControl1.TabIndex = 2;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.crobControl);
            this.tabPage2.Controls.Add(this.buttonSelectOperate);
            this.tabPage2.Controls.Add(this.buttonDirectOperate);
            this.tabPage2.Location = new System.Drawing.Point(23, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(259, 470);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "CROB";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // crobControl
            // 
            this.crobControl.Location = new System.Drawing.Point(17, 19);
            this.crobControl.Name = "crobControl";
            this.crobControl.Size = new System.Drawing.Size(225, 224);
            this.crobControl.TabIndex = 3;
            // 
            // buttonSelectOperate
            // 
            this.buttonSelectOperate.Location = new System.Drawing.Point(138, 258);
            this.buttonSelectOperate.Name = "buttonSelectOperate";
            this.buttonSelectOperate.Size = new System.Drawing.Size(104, 47);
            this.buttonSelectOperate.TabIndex = 2;
            this.buttonSelectOperate.Text = "Select and Operate";
            this.buttonSelectOperate.UseVisualStyleBackColor = true;
            this.buttonSelectOperate.Click += new System.EventHandler(this.buttonSelectOperate_Click);
            // 
            // buttonDirectOperate
            // 
            this.buttonDirectOperate.Location = new System.Drawing.Point(17, 258);
            this.buttonDirectOperate.Name = "buttonDirectOperate";
            this.buttonDirectOperate.Size = new System.Drawing.Size(95, 47);
            this.buttonDirectOperate.TabIndex = 1;
            this.buttonDirectOperate.Text = "Direct Operate";
            this.buttonDirectOperate.UseVisualStyleBackColor = true;
            this.buttonDirectOperate.Click += new System.EventHandler(this.buttonDirectOperate_Click);
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.analogOutputControl);
            this.tabPage3.Controls.Add(this.buttonSBOAO);
            this.tabPage3.Controls.Add(this.buttonDOAO);
            this.tabPage3.Location = new System.Drawing.Point(23, 4);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(259, 470);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "AO";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // analogOutputControl
            // 
            this.analogOutputControl.Location = new System.Drawing.Point(18, 20);
            this.analogOutputControl.Name = "analogOutputControl";
            this.analogOutputControl.Size = new System.Drawing.Size(225, 165);
            this.analogOutputControl.TabIndex = 7;
            // 
            // buttonSBOAO
            // 
            this.buttonSBOAO.Location = new System.Drawing.Point(139, 205);
            this.buttonSBOAO.Name = "buttonSBOAO";
            this.buttonSBOAO.Size = new System.Drawing.Size(104, 47);
            this.buttonSBOAO.TabIndex = 6;
            this.buttonSBOAO.Text = "Select and Operate";
            this.buttonSBOAO.UseVisualStyleBackColor = true;
            this.buttonSBOAO.Click += new System.EventHandler(this.buttonSBOAO_Click);
            // 
            // buttonDOAO
            // 
            this.buttonDOAO.Location = new System.Drawing.Point(18, 205);
            this.buttonDOAO.Name = "buttonDOAO";
            this.buttonDOAO.Size = new System.Drawing.Size(95, 47);
            this.buttonDOAO.TabIndex = 5;
            this.buttonDOAO.Text = "Direct Operate";
            this.buttonDOAO.UseVisualStyleBackColor = true;
            this.buttonDOAO.Click += new System.EventHandler(this.buttonDOAO_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.comboBoxFunctionCode);
            this.tabPage4.Controls.Add(this.buttonSendFunction);
            this.tabPage4.Location = new System.Drawing.Point(23, 4);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(259, 470);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "Custom";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // comboBoxFunctionCode
            // 
            this.comboBoxFunctionCode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFunctionCode.FormattingEnabled = true;
            this.comboBoxFunctionCode.Location = new System.Drawing.Point(26, 29);
            this.comboBoxFunctionCode.Name = "comboBoxFunctionCode";
            this.comboBoxFunctionCode.Size = new System.Drawing.Size(203, 21);
            this.comboBoxFunctionCode.TabIndex = 8;
            // 
            // buttonSendFunction
            // 
            this.buttonSendFunction.Location = new System.Drawing.Point(26, 83);
            this.buttonSendFunction.Name = "buttonSendFunction";
            this.buttonSendFunction.Size = new System.Drawing.Size(203, 47);
            this.buttonSendFunction.TabIndex = 7;
            this.buttonSendFunction.Text = "Send";
            this.buttonSendFunction.UseVisualStyleBackColor = true;
            this.buttonSendFunction.Click += new System.EventHandler(this.buttonSendFunction_Click);
            // 
            // comboBoxTypes
            // 
            this.comboBoxTypes.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxTypes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTypes.FormattingEnabled = true;
            this.comboBoxTypes.Location = new System.Drawing.Point(5, 5);
            this.comboBoxTypes.Name = "comboBoxTypes";
            this.comboBoxTypes.Size = new System.Drawing.Size(286, 21);
            this.comboBoxTypes.TabIndex = 0;
            this.comboBoxTypes.SelectedIndexChanged += new System.EventHandler(this.comboBoxTypes_SelectedIndexChanged);
            // 
            // measurementView
            // 
            this.measurementView.AllowSelection = false;
            this.measurementView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.measurementView.Location = new System.Drawing.Point(0, 0);
            this.measurementView.Name = "measurementView";
            this.measurementView.Size = new System.Drawing.Size(748, 519);
            this.measurementView.TabIndex = 2;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 497);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1048, 22);
            this.statusStrip1.TabIndex = 5;
            this.statusStrip1.Text = "statusStrip";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // MasterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1048, 519);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.splitContainer1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MasterForm";
            this.Text = "Master";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.GUIMasterForm_FormClosing);
            this.Load += new System.EventHandler(this.GUIMasterForm_Load);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ComboBox comboBoxTypes;
        private Automatak.Simulator.DNP3.Commons.MeasurementView measurementView;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private Components.MasterScanControl masterScanControl1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button buttonDirectOperate;
        private System.Windows.Forms.Button buttonSelectOperate;
        private CROBControl crobControl;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Button buttonSBOAO;
        private System.Windows.Forms.Button buttonDOAO;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private Components.AnalogOutputControl analogOutputControl;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Button buttonSendFunction;
        private System.Windows.Forms.ComboBox comboBoxFunctionCode;

    }
}