namespace Automatak.Simulator.DNP3.Components
{
    partial class CommandHandlerControl
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
            this.listBoxLog = new System.Windows.Forms.ListBox();
            this.groupBoxBehavior = new System.Windows.Forms.GroupBox();
            this.checkBoxEnabled = new System.Windows.Forms.CheckBox();
            this.checkBoxMapAnalog = new System.Windows.Forms.CheckBox();
            this.checkBoxMapBinary = new System.Windows.Forms.CheckBox();
            this.listBoxHandlers = new System.Windows.Forms.ListBox();
            this.buttonAddBO = new System.Windows.Forms.Button();
            this.buttonReset = new System.Windows.Forms.Button();
            this.buttonAddAO = new System.Windows.Forms.Button();
            this.groupBoxBehavior.SuspendLayout();
            this.SuspendLayout();
            // 
            // listBoxLog
            // 
            this.listBoxLog.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxLog.FormattingEnabled = true;
            this.listBoxLog.Location = new System.Drawing.Point(451, 13);
            this.listBoxLog.Name = "listBoxLog";
            this.listBoxLog.Size = new System.Drawing.Size(282, 264);
            this.listBoxLog.TabIndex = 2;
            // 
            // groupBoxBehavior
            // 
            this.groupBoxBehavior.Controls.Add(this.checkBoxEnabled);
            this.groupBoxBehavior.Controls.Add(this.checkBoxMapAnalog);
            this.groupBoxBehavior.Controls.Add(this.checkBoxMapBinary);
            this.groupBoxBehavior.Location = new System.Drawing.Point(12, 13);
            this.groupBoxBehavior.Name = "groupBoxBehavior";
            this.groupBoxBehavior.Size = new System.Drawing.Size(170, 264);
            this.groupBoxBehavior.TabIndex = 10;
            this.groupBoxBehavior.TabStop = false;
            this.groupBoxBehavior.Text = "Command Behavior";
            // 
            // checkBoxEnabled
            // 
            this.checkBoxEnabled.AutoSize = true;
            this.checkBoxEnabled.Location = new System.Drawing.Point(21, 30);
            this.checkBoxEnabled.Name = "checkBoxEnabled";
            this.checkBoxEnabled.Size = new System.Drawing.Size(123, 17);
            this.checkBoxEnabled.TabIndex = 2;
            this.checkBoxEnabled.Text = "Intercept Commands";
            this.checkBoxEnabled.UseVisualStyleBackColor = true;
            this.checkBoxEnabled.CheckedChanged += new System.EventHandler(this.checkBoxEnabled_CheckedChanged);
            // 
            // checkBoxMapAnalog
            // 
            this.checkBoxMapAnalog.AutoSize = true;
            this.checkBoxMapAnalog.Location = new System.Drawing.Point(21, 60);
            this.checkBoxMapAnalog.Name = "checkBoxMapAnalog";
            this.checkBoxMapAnalog.Size = new System.Drawing.Size(128, 17);
            this.checkBoxMapAnalog.TabIndex = 1;
            this.checkBoxMapAnalog.Text = "Map AO to AO Status";
            this.checkBoxMapAnalog.UseVisualStyleBackColor = true;
            // 
            // checkBoxMapBinary
            // 
            this.checkBoxMapBinary.AutoSize = true;
            this.checkBoxMapBinary.Location = new System.Drawing.Point(21, 91);
            this.checkBoxMapBinary.Name = "checkBoxMapBinary";
            this.checkBoxMapBinary.Size = new System.Drawing.Size(140, 17);
            this.checkBoxMapBinary.TabIndex = 0;
            this.checkBoxMapBinary.Text = "Map Latch to BO Status";
            this.checkBoxMapBinary.UseVisualStyleBackColor = true;
            // 
            // listBoxHandlers
            // 
            this.listBoxHandlers.FormattingEnabled = true;
            this.listBoxHandlers.Location = new System.Drawing.Point(204, 13);
            this.listBoxHandlers.Name = "listBoxHandlers";
            this.listBoxHandlers.Size = new System.Drawing.Size(232, 212);
            this.listBoxHandlers.TabIndex = 11;
            // 
            // buttonAddBO
            // 
            this.buttonAddBO.Location = new System.Drawing.Point(204, 238);
            this.buttonAddBO.Name = "buttonAddBO";
            this.buttonAddBO.Size = new System.Drawing.Size(72, 39);
            this.buttonAddBO.TabIndex = 12;
            this.buttonAddBO.Text = "Add BO";
            this.buttonAddBO.UseVisualStyleBackColor = true;
            this.buttonAddBO.Click += new System.EventHandler(this.buttonAddBO_Click);
            // 
            // buttonReset
            // 
            this.buttonReset.Location = new System.Drawing.Point(364, 238);
            this.buttonReset.Name = "buttonReset";
            this.buttonReset.Size = new System.Drawing.Size(72, 39);
            this.buttonReset.TabIndex = 13;
            this.buttonReset.Text = "Reset";
            this.buttonReset.UseVisualStyleBackColor = true;
            this.buttonReset.Click += new System.EventHandler(this.buttonReset_Click);
            // 
            // buttonAddAO
            // 
            this.buttonAddAO.Location = new System.Drawing.Point(284, 238);
            this.buttonAddAO.Name = "buttonAddAO";
            this.buttonAddAO.Size = new System.Drawing.Size(72, 39);
            this.buttonAddAO.TabIndex = 14;
            this.buttonAddAO.Text = "Add AO";
            this.buttonAddAO.UseVisualStyleBackColor = true;
            this.buttonAddAO.Click += new System.EventHandler(this.buttonAddAO_Click);
            // 
            // CommandHandlerControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.buttonAddAO);
            this.Controls.Add(this.buttonReset);
            this.Controls.Add(this.buttonAddBO);
            this.Controls.Add(this.listBoxHandlers);
            this.Controls.Add(this.groupBoxBehavior);
            this.Controls.Add(this.listBoxLog);
            this.Name = "CommandHandlerControl";
            this.Size = new System.Drawing.Size(755, 297);
            this.groupBoxBehavior.ResumeLayout(false);
            this.groupBoxBehavior.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox listBoxLog;
        private System.Windows.Forms.GroupBox groupBoxBehavior;
        private System.Windows.Forms.ListBox listBoxHandlers;
        private System.Windows.Forms.Button buttonAddBO;
        private System.Windows.Forms.Button buttonReset;
        private System.Windows.Forms.CheckBox checkBoxMapAnalog;
        private System.Windows.Forms.CheckBox checkBoxMapBinary;
        private System.Windows.Forms.CheckBox checkBoxEnabled;
        private System.Windows.Forms.Button buttonAddAO;
    }
}
