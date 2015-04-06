namespace Automatak.Simulator.DNP3.Components
{
    partial class OutstationModuleDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OutstationModuleDialog));
            this.buttonOK = new System.Windows.Forms.Button();
            this.comboBoxModules = new System.Windows.Forms.ComboBox();
            this.textBoxDesc = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(25, 153);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(290, 44);
            this.buttonOK.TabIndex = 0;
            this.buttonOK.Text = "Select";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // comboBoxModules
            // 
            this.comboBoxModules.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxModules.FormattingEnabled = true;
            this.comboBoxModules.Location = new System.Drawing.Point(25, 25);
            this.comboBoxModules.Name = "comboBoxModules";
            this.comboBoxModules.Size = new System.Drawing.Size(290, 21);
            this.comboBoxModules.TabIndex = 1;
            this.comboBoxModules.SelectedIndexChanged += new System.EventHandler(this.comboBoxModules_SelectedIndexChanged);
            // 
            // textBoxDesc
            // 
            this.textBoxDesc.Location = new System.Drawing.Point(25, 58);
            this.textBoxDesc.Multiline = true;
            this.textBoxDesc.Name = "textBoxDesc";
            this.textBoxDesc.ReadOnly = true;
            this.textBoxDesc.Size = new System.Drawing.Size(290, 83);
            this.textBoxDesc.TabIndex = 2;
            // 
            // OutstationModuleDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(342, 209);
            this.Controls.Add(this.textBoxDesc);
            this.Controls.Add(this.comboBoxModules);
            this.Controls.Add(this.buttonOK);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OutstationModuleDialog";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Choose an outstation plugin";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.ComboBox comboBoxModules;
        private System.Windows.Forms.TextBox textBoxDesc;
    }
}