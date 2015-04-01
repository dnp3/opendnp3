using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    partial class ScanDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ScanDialog));
            this.buttonOnce = new System.Windows.Forms.Button();
            this.buttonBind = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.numericUpDownPeriod = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.classFieldControlScan = new Automatak.Simulator.DNP3.Components.ClassFieldControl();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPeriod)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonOnce
            // 
            this.buttonOnce.Location = new System.Drawing.Point(27, 274);
            this.buttonOnce.Name = "buttonOnce";
            this.buttonOnce.Size = new System.Drawing.Size(146, 48);
            this.buttonOnce.TabIndex = 0;
            this.buttonOnce.Text = "Scan Once";
            this.buttonOnce.UseVisualStyleBackColor = true;
            this.buttonOnce.Click += new System.EventHandler(this.buttonOnce_Click);
            // 
            // buttonBind
            // 
            this.buttonBind.Location = new System.Drawing.Point(200, 274);
            this.buttonBind.Name = "buttonBind";
            this.buttonBind.Size = new System.Drawing.Size(150, 48);
            this.buttonBind.TabIndex = 1;
            this.buttonBind.Text = "Add Scan";
            this.buttonBind.UseVisualStyleBackColor = true;
            this.buttonBind.Click += new System.EventHandler(this.buttonBind_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(27, 18);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(327, 230);
            this.tabControl1.TabIndex = 2;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.numericUpDownPeriod);
            this.tabPage1.Controls.Add(this.classFieldControlScan);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(319, 204);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Class Scan";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // numericUpDownPeriod
            // 
            this.numericUpDownPeriod.Location = new System.Drawing.Point(22, 153);
            this.numericUpDownPeriod.Maximum = new decimal(new int[] {
            60000,
            0,
            0,
            0});
            this.numericUpDownPeriod.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.numericUpDownPeriod.Name = "numericUpDownPeriod";
            this.numericUpDownPeriod.Size = new System.Drawing.Size(93, 20);
            this.numericUpDownPeriod.TabIndex = 1;
            this.numericUpDownPeriod.Value = new decimal(new int[] {
            5000,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(131, 155);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Period";
            // 
            // classFieldControlScan
            // 
            this.classFieldControlScan.ClassFieldValue = ClassField.None;
            this.classFieldControlScan.Location = new System.Drawing.Point(22, 23);
            this.classFieldControlScan.Name = "classFieldControlScan";
            this.classFieldControlScan.Size = new System.Drawing.Size(77, 112);
            this.classFieldControlScan.TabIndex = 0;
            // 
            // ScanDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(385, 346);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.buttonBind);
            this.Controls.Add(this.buttonOnce);            
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ScanDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Configure Scans";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownPeriod)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonOnce;
        private System.Windows.Forms.Button buttonBind;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDownPeriod;
        private ClassFieldControl classFieldControlScan;
    }
}