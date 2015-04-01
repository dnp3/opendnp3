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
            this.comboBoxTypes = new System.Windows.Forms.ComboBox();
            this.measurementView = new Automatak.Simulator.DNP3.MeasurementView();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.masterScanControl1 = new Automatak.Simulator.DNP3.Components.MasterScanControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
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
            this.splitContainer1.Size = new System.Drawing.Size(1056, 592);
            this.splitContainer1.SplitterDistance = 359;
            this.splitContainer1.TabIndex = 1;
            // 
            // comboBoxTypes
            // 
            this.comboBoxTypes.Dock = System.Windows.Forms.DockStyle.Top;
            this.comboBoxTypes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTypes.FormattingEnabled = true;
            this.comboBoxTypes.Location = new System.Drawing.Point(5, 5);
            this.comboBoxTypes.Name = "comboBoxTypes";
            this.comboBoxTypes.Size = new System.Drawing.Size(349, 21);
            this.comboBoxTypes.TabIndex = 0;
            this.comboBoxTypes.SelectedIndexChanged += new System.EventHandler(this.comboBoxTypes_SelectedIndexChanged);
            // 
            // measurementView
            // 
            this.measurementView.AllowSelection = false;
            this.measurementView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.measurementView.Location = new System.Drawing.Point(0, 0);
            this.measurementView.Name = "measurementView";
            this.measurementView.Size = new System.Drawing.Size(693, 592);
            this.measurementView.TabIndex = 2;
            // 
            // tabControl1
            // 
            this.tabControl1.Alignment = System.Windows.Forms.TabAlignment.Left;
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(5, 32);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(349, 551);
            this.tabControl1.TabIndex = 3;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.masterScanControl1);
            this.tabPage1.Location = new System.Drawing.Point(23, 4);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(322, 543);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Scans";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // masterScanControl1
            // 
            this.masterScanControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.masterScanControl1.Location = new System.Drawing.Point(3, 3);
            this.masterScanControl1.Name = "masterScanControl1";
            this.masterScanControl1.Size = new System.Drawing.Size(316, 537);
            this.masterScanControl1.TabIndex = 2;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(23, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(322, 543);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "CROB";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // MasterForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1056, 592);
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
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ComboBox comboBoxTypes;
        private MeasurementView measurementView;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private Components.MasterScanControl masterScanControl1;
        private System.Windows.Forms.TabPage tabPage2;

    }
}