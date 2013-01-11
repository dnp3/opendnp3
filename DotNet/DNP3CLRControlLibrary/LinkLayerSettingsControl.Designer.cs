namespace TestSetControlLibrary
{
    partial class LinkLayerSettingsControl
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.checkBoxUseConfirms = new System.Windows.Forms.CheckBox();
            this.numericUpDownRetries = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownLocalAddress = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.numericUpDownRemoteAddress = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.numericUpDownTimeout = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRetries)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownLocalAddress)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRemoteAddress)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTimeout)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.numericUpDownTimeout);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.numericUpDownRemoteAddress);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.numericUpDownLocalAddress);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.numericUpDownRetries);
            this.groupBox1.Controls.Add(this.checkBoxUseConfirms);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(280, 186);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Link Layer";
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // checkBoxUseConfirms
            // 
            this.checkBoxUseConfirms.AutoSize = true;
            this.checkBoxUseConfirms.Location = new System.Drawing.Point(148, 33);
            this.checkBoxUseConfirms.Name = "checkBoxUseConfirms";
            this.checkBoxUseConfirms.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.checkBoxUseConfirms.Size = new System.Drawing.Size(15, 14);
            this.checkBoxUseConfirms.TabIndex = 0;
            this.checkBoxUseConfirms.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.checkBoxUseConfirms.UseVisualStyleBackColor = true;
            // 
            // numericUpDownRetries
            // 
            this.numericUpDownRetries.Location = new System.Drawing.Point(149, 56);
            this.numericUpDownRetries.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.numericUpDownRetries.Name = "numericUpDownRetries";
            this.numericUpDownRetries.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericUpDownRetries.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownRetries.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(48, 59);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Num Retries";
            // 
            // numericUpDownLocalAddress
            // 
            this.numericUpDownLocalAddress.Location = new System.Drawing.Point(148, 85);
            this.numericUpDownLocalAddress.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownLocalAddress.Name = "numericUpDownLocalAddress";
            this.numericUpDownLocalAddress.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericUpDownLocalAddress.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownLocalAddress.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(48, 34);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(69, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Use Confirms";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(48, 87);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(74, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Local Address";
            // 
            // numericUpDownRemoteAddress
            // 
            this.numericUpDownRemoteAddress.Location = new System.Drawing.Point(148, 113);
            this.numericUpDownRemoteAddress.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownRemoteAddress.Name = "numericUpDownRemoteAddress";
            this.numericUpDownRemoteAddress.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericUpDownRemoteAddress.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownRemoteAddress.TabIndex = 6;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(48, 115);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(85, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Remote Address";
            // 
            // numericUpDownTimeout
            // 
            this.numericUpDownTimeout.Location = new System.Drawing.Point(148, 139);
            this.numericUpDownTimeout.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDownTimeout.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownTimeout.Name = "numericUpDownTimeout";
            this.numericUpDownTimeout.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericUpDownTimeout.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownTimeout.TabIndex = 8;
            this.numericUpDownTimeout.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(48, 141);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Timeout (ms)";
            // 
            // LinkLayerSettingsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "LinkLayerSettingsControl";
            this.Size = new System.Drawing.Size(280, 186);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRetries)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownLocalAddress)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRemoteAddress)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTimeout)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.NumericUpDown numericUpDownTimeout;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numericUpDownRemoteAddress;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numericUpDownLocalAddress;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownRetries;
        private System.Windows.Forms.CheckBox checkBoxUseConfirms;


    }
}
