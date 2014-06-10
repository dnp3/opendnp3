namespace Automatak.DNP3.Simulator.Components
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDownDest = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownSource = new System.Windows.Forms.NumericUpDown();
            this.checkBoxConfirmed = new System.Windows.Forms.CheckBox();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownDest)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.numericUpDown1);
            this.groupBox1.Controls.Add(this.checkBoxConfirmed);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.numericUpDownDest);
            this.groupBox1.Controls.Add(this.numericUpDownSource);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(270, 441);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Link Settings";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(112, 58);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "destination";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(112, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(39, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "source";
            // 
            // numericUpDownDest
            // 
            this.numericUpDownDest.Increment = new decimal(new int[] {
            250,
            0,
            0,
            0});
            this.numericUpDownDest.Location = new System.Drawing.Point(18, 55);
            this.numericUpDownDest.Maximum = new decimal(new int[] {
            65530,
            0,
            0,
            0});
            this.numericUpDownDest.Name = "numericUpDownDest";
            this.numericUpDownDest.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownDest.TabIndex = 9;
            this.numericUpDownDest.Value = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            // 
            // numericUpDownSource
            // 
            this.numericUpDownSource.Location = new System.Drawing.Point(18, 28);
            this.numericUpDownSource.Maximum = new decimal(new int[] {
            65530,
            0,
            0,
            0});
            this.numericUpDownSource.Name = "numericUpDownSource";
            this.numericUpDownSource.Size = new System.Drawing.Size(88, 20);
            this.numericUpDownSource.TabIndex = 8;
            this.numericUpDownSource.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // checkBoxConfirmed
            // 
            this.checkBoxConfirmed.AutoSize = true;
            this.checkBoxConfirmed.Location = new System.Drawing.Point(18, 86);
            this.checkBoxConfirmed.Name = "checkBoxConfirmed";
            this.checkBoxConfirmed.Size = new System.Drawing.Size(145, 17);
            this.checkBoxConfirmed.TabIndex = 12;
            this.checkBoxConfirmed.Text = "send confirmed user data";
            this.checkBoxConfirmed.UseVisualStyleBackColor = true;
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDown1.Location = new System.Drawing.Point(18, 110);
            this.numericUpDown1.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDown1.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(88, 20);
            this.numericUpDown1.TabIndex = 13;
            this.numericUpDown1.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(112, 112);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "timeout (ms)";
            // 
            // LinkConfigControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "LinkConfigControl";
            this.Size = new System.Drawing.Size(270, 441);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownDest)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownDest;
        private System.Windows.Forms.NumericUpDown numericUpDownSource;
        private System.Windows.Forms.CheckBox checkBoxConfirmed;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numericUpDown1;

    }
}
