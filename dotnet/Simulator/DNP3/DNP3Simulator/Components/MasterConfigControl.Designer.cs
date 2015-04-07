using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    partial class MasterConfigControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MasterConfigControl));
            this.comboBoxTimeSync = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.checkBoxDisableUnsolicited = new System.Windows.Forms.CheckBox();
            this.groupBoxIntegrity = new System.Windows.Forms.GroupBox();
            this.classFieldControlIntegrity = new Automatak.Simulator.DNP3.Components.ClassFieldControl();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.classFieldControlEnableUnsol = new Automatak.Simulator.DNP3.Components.ClassFieldControl();
            this.checkBoxIntegrityOnOverflow = new System.Windows.Forms.CheckBox();
            this.numericUpDownResponseTimeout = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.numericUpDownTaskRetryDelay = new System.Windows.Forms.NumericUpDown();
            this.groupBoxIntegrity.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownResponseTimeout)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTaskRetryDelay)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBoxTimeSync
            // 
            this.comboBoxTimeSync.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTimeSync.FormattingEnabled = true;
            this.comboBoxTimeSync.Location = new System.Drawing.Point(18, 252);
            this.comboBoxTimeSync.Name = "comboBoxTimeSync";
            this.comboBoxTimeSync.Size = new System.Drawing.Size(119, 21);
            this.comboBoxTimeSync.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(143, 255);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(87, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Time Sync Mode";
            // 
            // checkBoxDisableUnsolicited
            // 
            this.checkBoxDisableUnsolicited.AutoSize = true;
            this.checkBoxDisableUnsolicited.Checked = true;
            this.checkBoxDisableUnsolicited.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxDisableUnsolicited.Location = new System.Drawing.Point(20, 22);
            this.checkBoxDisableUnsolicited.Name = "checkBoxDisableUnsolicited";
            this.checkBoxDisableUnsolicited.Size = new System.Drawing.Size(170, 17);
            this.checkBoxDisableUnsolicited.TabIndex = 2;
            this.checkBoxDisableUnsolicited.Text = "Disable Unsolicited On Startup";
            this.checkBoxDisableUnsolicited.UseVisualStyleBackColor = true;
            // 
            // groupBoxIntegrity
            // 
            this.groupBoxIntegrity.Controls.Add(this.classFieldControlIntegrity);
            this.groupBoxIntegrity.Location = new System.Drawing.Point(18, 61);
            this.groupBoxIntegrity.Name = "groupBoxIntegrity";
            this.groupBoxIntegrity.Size = new System.Drawing.Size(119, 138);
            this.groupBoxIntegrity.TabIndex = 4;
            this.groupBoxIntegrity.TabStop = false;
            this.groupBoxIntegrity.Text = "Startup Integrity";
            // 
            // classFieldControlIntegrity
            // 
            this.classFieldControlIntegrity.ClassFieldValue = ((Automatak.DNP3.Interface.ClassField)(resources.GetObject("classFieldControlIntegrity.ClassFieldValue")));
            this.classFieldControlIntegrity.Location = new System.Drawing.Point(6, 19);
            this.classFieldControlIntegrity.Name = "classFieldControlIntegrity";
            this.classFieldControlIntegrity.Size = new System.Drawing.Size(77, 112);
            this.classFieldControlIntegrity.TabIndex = 3;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.classFieldControlEnableUnsol);
            this.groupBox1.Location = new System.Drawing.Point(151, 61);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(116, 138);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Enable Unsolicited";
            // 
            // classFieldControlEnableUnsol
            // 
            this.classFieldControlEnableUnsol.ClassFieldValue = ((Automatak.DNP3.Interface.ClassField)(resources.GetObject("classFieldControlEnableUnsol.ClassFieldValue")));
            this.classFieldControlEnableUnsol.Location = new System.Drawing.Point(6, 19);
            this.classFieldControlEnableUnsol.Name = "classFieldControlEnableUnsol";
            this.classFieldControlEnableUnsol.Size = new System.Drawing.Size(77, 112);
            this.classFieldControlEnableUnsol.TabIndex = 3;
            // 
            // checkBoxIntegrityOnOverflow
            // 
            this.checkBoxIntegrityOnOverflow.AutoSize = true;
            this.checkBoxIntegrityOnOverflow.Checked = true;
            this.checkBoxIntegrityOnOverflow.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxIntegrityOnOverflow.Location = new System.Drawing.Point(18, 215);
            this.checkBoxIntegrityOnOverflow.Name = "checkBoxIntegrityOnOverflow";
            this.checkBoxIntegrityOnOverflow.Size = new System.Drawing.Size(167, 17);
            this.checkBoxIntegrityOnOverflow.TabIndex = 6;
            this.checkBoxIntegrityOnOverflow.Text = "Auto Integrity On IIN Overflow";
            this.checkBoxIntegrityOnOverflow.UseVisualStyleBackColor = true;
            // 
            // numericUpDownResponseTimeout
            // 
            this.numericUpDownResponseTimeout.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDownResponseTimeout.Location = new System.Drawing.Point(18, 292);
            this.numericUpDownResponseTimeout.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownResponseTimeout.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownResponseTimeout.Name = "numericUpDownResponseTimeout";
            this.numericUpDownResponseTimeout.Size = new System.Drawing.Size(83, 20);
            this.numericUpDownResponseTimeout.TabIndex = 7;
            this.numericUpDownResponseTimeout.Value = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(107, 294);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Respone Timeout (ms)";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(107, 330);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(111, 13);
            this.label3.TabIndex = 10;
            this.label3.Text = "Task Retry Delay (ms)";
            // 
            // numericUpDownTaskRetryDelay
            // 
            this.numericUpDownTaskRetryDelay.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDownTaskRetryDelay.Location = new System.Drawing.Point(18, 328);
            this.numericUpDownTaskRetryDelay.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownTaskRetryDelay.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownTaskRetryDelay.Name = "numericUpDownTaskRetryDelay";
            this.numericUpDownTaskRetryDelay.Size = new System.Drawing.Size(83, 20);
            this.numericUpDownTaskRetryDelay.TabIndex = 9;
            this.numericUpDownTaskRetryDelay.Value = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            // 
            // MasterConfigControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numericUpDownTaskRetryDelay);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.numericUpDownResponseTimeout);
            this.Controls.Add(this.checkBoxIntegrityOnOverflow);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBoxIntegrity);
            this.Controls.Add(this.checkBoxDisableUnsolicited);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBoxTimeSync);
            this.Name = "MasterConfigControl";
            this.Size = new System.Drawing.Size(283, 370);
            this.groupBoxIntegrity.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownResponseTimeout)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTaskRetryDelay)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBoxTimeSync;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox checkBoxDisableUnsolicited;
        private ClassFieldControl classFieldControlIntegrity;
        private System.Windows.Forms.GroupBox groupBoxIntegrity;
        private System.Windows.Forms.GroupBox groupBox1;
        private ClassFieldControl classFieldControlEnableUnsol;
        private System.Windows.Forms.CheckBox checkBoxIntegrityOnOverflow;
        private System.Windows.Forms.NumericUpDown numericUpDownResponseTimeout;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericUpDownTaskRetryDelay;
    }
}
