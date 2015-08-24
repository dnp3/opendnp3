namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{
    partial class OutstationForm
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.listBoxEvents = new System.Windows.Forms.ListBox();
            this.buttonClear = new System.Windows.Forms.Button();
            this.buttonApply = new System.Windows.Forms.Button();
            this.buttonEdit = new System.Windows.Forms.Button();
            this.comboBoxTypes = new System.Windows.Forms.ComboBox();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.measurementView = new Automatak.Simulator.DNP3.Commons.MeasurementView();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxColdRestartMode = new System.Windows.Forms.ComboBox();
            this.numericUpDownColdRestartTime = new System.Windows.Forms.NumericUpDown();
            this.checkBoxLocalMode = new System.Windows.Forms.CheckBox();
            this.checkBoxNeedTime = new System.Windows.Forms.CheckBox();
            this.commandHandlerControl1 = new Automatak.Simulator.DNP3.DefaultOutstationPlugin.CommandHandlerControl();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownColdRestartTime)).BeginInit();
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
            this.splitContainer1.Panel1.Controls.Add(this.listBoxEvents);
            this.splitContainer1.Panel1.Controls.Add(this.buttonClear);
            this.splitContainer1.Panel1.Controls.Add(this.buttonApply);
            this.splitContainer1.Panel1.Controls.Add(this.buttonEdit);
            this.splitContainer1.Panel1.Controls.Add(this.comboBoxTypes);
            this.splitContainer1.Panel1.Padding = new System.Windows.Forms.Padding(5);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(1141, 563);
            this.splitContainer1.SplitterDistance = 260;
            this.splitContainer1.TabIndex = 1;
            // 
            // listBoxEvents
            // 
            this.listBoxEvents.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxEvents.FormattingEnabled = true;
            this.listBoxEvents.Location = new System.Drawing.Point(8, 209);
            this.listBoxEvents.Name = "listBoxEvents";
            this.listBoxEvents.Size = new System.Drawing.Size(244, 342);
            this.listBoxEvents.TabIndex = 0;
            // 
            // buttonClear
            // 
            this.buttonClear.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClear.Location = new System.Drawing.Point(5, 137);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(247, 38);
            this.buttonClear.TabIndex = 3;
            this.buttonClear.Text = "Clear Events";
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.buttonClear_Click);
            // 
            // buttonApply
            // 
            this.buttonApply.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonApply.Location = new System.Drawing.Point(5, 93);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(247, 38);
            this.buttonApply.TabIndex = 2;
            this.buttonApply.Text = "Apply Events";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.buttonApply_Click);
            // 
            // buttonEdit
            // 
            this.buttonEdit.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonEdit.Location = new System.Drawing.Point(5, 49);
            this.buttonEdit.Name = "buttonEdit";
            this.buttonEdit.Size = new System.Drawing.Size(247, 38);
            this.buttonEdit.TabIndex = 1;
            this.buttonEdit.Text = "Edit Selection";
            this.buttonEdit.UseVisualStyleBackColor = true;
            this.buttonEdit.Click += new System.EventHandler(this.buttonEdit_Click);
            // 
            // comboBoxTypes
            // 
            this.comboBoxTypes.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxTypes.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTypes.FormattingEnabled = true;
            this.comboBoxTypes.Location = new System.Drawing.Point(5, 5);
            this.comboBoxTypes.Name = "comboBoxTypes";
            this.comboBoxTypes.Size = new System.Drawing.Size(247, 21);
            this.comboBoxTypes.TabIndex = 0;
            this.comboBoxTypes.SelectedIndexChanged += new System.EventHandler(this.comboBoxTypes_SelectedIndexChanged);
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.measurementView);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.groupBox1);
            this.splitContainer2.Panel2.Controls.Add(this.commandHandlerControl1);
            this.splitContainer2.Size = new System.Drawing.Size(877, 563);
            this.splitContainer2.SplitterDistance = 268;
            this.splitContainer2.TabIndex = 0;
            // 
            // measurementView
            // 
            this.measurementView.AllowSelection = true;
            this.measurementView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.measurementView.Location = new System.Drawing.Point(0, 0);
            this.measurementView.Name = "measurementView";
            this.measurementView.Size = new System.Drawing.Size(877, 268);
            this.measurementView.TabIndex = 3;
            this.measurementView.OnRowSelectionChanged += new Automatak.Simulator.DNP3.Commons.MeasurementView.RowSelectionEvent(this.measurementView_OnRowSelectionChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.comboBoxColdRestartMode);
            this.groupBox1.Controls.Add(this.numericUpDownColdRestartTime);
            this.groupBox1.Controls.Add(this.checkBoxLocalMode);
            this.groupBox1.Controls.Add(this.checkBoxNeedTime);
            this.groupBox1.Location = new System.Drawing.Point(21, 13);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(163, 265);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Simulated Behavior";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(20, 148);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(95, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Cold Restart Delay";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 94);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(95, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Cold Restart Mode";
            // 
            // comboBoxColdRestartMode
            // 
            this.comboBoxColdRestartMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxColdRestartMode.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBoxColdRestartMode.FormattingEnabled = true;
            this.comboBoxColdRestartMode.Location = new System.Drawing.Point(18, 113);
            this.comboBoxColdRestartMode.Name = "comboBoxColdRestartMode";
            this.comboBoxColdRestartMode.Size = new System.Drawing.Size(131, 17);
            this.comboBoxColdRestartMode.TabIndex = 4;
            this.comboBoxColdRestartMode.SelectedValueChanged += new System.EventHandler(this.comboBoxColdRestartMode_SelectedValueChanged);
            // 
            // numericUpDownColdRestartTime
            // 
            this.numericUpDownColdRestartTime.Location = new System.Drawing.Point(18, 164);
            this.numericUpDownColdRestartTime.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownColdRestartTime.Name = "numericUpDownColdRestartTime";
            this.numericUpDownColdRestartTime.Size = new System.Drawing.Size(131, 20);
            this.numericUpDownColdRestartTime.TabIndex = 3;
            this.numericUpDownColdRestartTime.ValueChanged += new System.EventHandler(this.numericUpDownColdRestartTime_ValueChanged);
            // 
            // checkBoxLocalMode
            // 
            this.checkBoxLocalMode.AutoSize = true;
            this.checkBoxLocalMode.Location = new System.Drawing.Point(20, 58);
            this.checkBoxLocalMode.Name = "checkBoxLocalMode";
            this.checkBoxLocalMode.Size = new System.Drawing.Size(129, 17);
            this.checkBoxLocalMode.TabIndex = 1;
            this.checkBoxLocalMode.Text = "Local Control (IIN 1.5)";
            this.checkBoxLocalMode.UseVisualStyleBackColor = true;
            this.checkBoxLocalMode.CheckedChanged += new System.EventHandler(this.checkBoxLocalMode_CheckedChanged);
            // 
            // checkBoxNeedTime
            // 
            this.checkBoxNeedTime.AutoSize = true;
            this.checkBoxNeedTime.Location = new System.Drawing.Point(20, 35);
            this.checkBoxNeedTime.Name = "checkBoxNeedTime";
            this.checkBoxNeedTime.Size = new System.Drawing.Size(119, 17);
            this.checkBoxNeedTime.TabIndex = 0;
            this.checkBoxNeedTime.Text = "Need Time (IIN 1.4)";
            this.checkBoxNeedTime.UseVisualStyleBackColor = true;
            this.checkBoxNeedTime.CheckedChanged += new System.EventHandler(this.checkBoxNeedTime_CheckedChanged);
            // 
            // commandHandlerControl1
            // 
            this.commandHandlerControl1.Location = new System.Drawing.Point(190, 2);
            this.commandHandlerControl1.Name = "commandHandlerControl1";
            this.commandHandlerControl1.Size = new System.Drawing.Size(694, 316);
            this.commandHandlerControl1.TabIndex = 0;
            // 
            // OutstationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1141, 563);
            this.Controls.Add(this.splitContainer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OutstationForm";
            this.ShowIcon = false;
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "Outstation";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.GUIMasterForm_FormClosing);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownColdRestartTime)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ComboBox comboBoxTypes;
        private System.Windows.Forms.Button buttonEdit;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private Automatak.Simulator.DNP3.Commons.MeasurementView measurementView;
        private System.Windows.Forms.Button buttonClear;
        private System.Windows.Forms.Button buttonApply;
        private System.Windows.Forms.ListBox listBoxEvents;
        private CommandHandlerControl commandHandlerControl1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxNeedTime;
        private System.Windows.Forms.CheckBox checkBoxLocalMode;
        private System.Windows.Forms.NumericUpDown numericUpDownColdRestartTime;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBoxColdRestartMode;

    }
}