namespace Automatak.Simulator.DNP3.Components
{
    partial class LinkConfigControl
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
            this.groupBoxConfirmed = new System.Windows.Forms.GroupBox();
            this.numericUpDownTimeout = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.numericUpDownRetries = new System.Windows.Forms.NumericUpDown();
            this.checkBoxConfirmed = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownDest = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownSource = new System.Windows.Forms.NumericUpDown();
            this.groupBoxConfirmed.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTimeout)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRetries)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownDest)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSource)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBoxConfirmed
            // 
            this.groupBoxConfirmed.Controls.Add(this.numericUpDownTimeout);
            this.groupBoxConfirmed.Controls.Add(this.label4);
            this.groupBoxConfirmed.Controls.Add(this.label3);
            this.groupBoxConfirmed.Controls.Add(this.numericUpDownRetries);
            this.groupBoxConfirmed.Location = new System.Drawing.Point(12, 105);
            this.groupBoxConfirmed.Name = "groupBoxConfirmed";
            this.groupBoxConfirmed.Size = new System.Drawing.Size(198, 100);
            this.groupBoxConfirmed.TabIndex = 23;
            this.groupBoxConfirmed.TabStop = false;
            this.groupBoxConfirmed.Text = "confirm options";
            // 
            // numericUpDownTimeout
            // 
            this.numericUpDownTimeout.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDownTimeout.Location = new System.Drawing.Point(15, 58);
            this.numericUpDownTimeout.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDownTimeout.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownTimeout.Name = "numericUpDownTimeout";
            this.numericUpDownTimeout.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownTimeout.TabIndex = 13;
            this.numericUpDownTimeout.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(109, 31);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 16;
            this.label4.Text = "retries";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(109, 60);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "timeout (ms)";
            // 
            // numericUpDownRetries
            // 
            this.numericUpDownRetries.Location = new System.Drawing.Point(15, 28);
            this.numericUpDownRetries.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownRetries.Name = "numericUpDownRetries";
            this.numericUpDownRetries.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownRetries.TabIndex = 15;
            // 
            // checkBoxConfirmed
            // 
            this.checkBoxConfirmed.AutoSize = true;
            this.checkBoxConfirmed.Location = new System.Drawing.Point(12, 76);
            this.checkBoxConfirmed.Name = "checkBoxConfirmed";
            this.checkBoxConfirmed.Size = new System.Drawing.Size(145, 17);
            this.checkBoxConfirmed.TabIndex = 22;
            this.checkBoxConfirmed.Text = "send confirmed user data";
            this.checkBoxConfirmed.UseVisualStyleBackColor = true;
            this.checkBoxConfirmed.CheckedChanged += new System.EventHandler(this.checkBoxConfirmed_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(106, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 13);
            this.label2.TabIndex = 21;
            this.label2.Text = "destination";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(106, 16);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(39, 13);
            this.label1.TabIndex = 20;
            this.label1.Text = "source";
            // 
            // numericUpDownDest
            // 
            this.numericUpDownDest.Location = new System.Drawing.Point(12, 41);
            this.numericUpDownDest.Maximum = new decimal(new int[] {
            65530,
            0,
            0,
            0});
            this.numericUpDownDest.Name = "numericUpDownDest";
            this.numericUpDownDest.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownDest.TabIndex = 19;
            this.numericUpDownDest.Value = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            // 
            // numericUpDownSource
            // 
            this.numericUpDownSource.Location = new System.Drawing.Point(12, 14);
            this.numericUpDownSource.Maximum = new decimal(new int[] {
            65530,
            0,
            0,
            0});
            this.numericUpDownSource.Name = "numericUpDownSource";
            this.numericUpDownSource.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownSource.TabIndex = 18;
            this.numericUpDownSource.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // LinkConfigControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBoxConfirmed);
            this.Controls.Add(this.checkBoxConfirmed);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.numericUpDownDest);
            this.Controls.Add(this.numericUpDownSource);
            this.Name = "LinkConfigControl";
            this.Size = new System.Drawing.Size(232, 236);
            this.groupBoxConfirmed.ResumeLayout(false);
            this.groupBoxConfirmed.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTimeout)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownRetries)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownDest)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxConfirmed;
        private System.Windows.Forms.NumericUpDown numericUpDownTimeout;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericUpDownRetries;
        private System.Windows.Forms.CheckBox checkBoxConfirmed;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownDest;
        private System.Windows.Forms.NumericUpDown numericUpDownSource;


    }
}
