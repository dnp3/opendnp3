namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
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
            this.components = new System.ComponentModel.Container();
            this.listBoxLog = new System.Windows.Forms.ListBox();
            this.contextMenuStripOperations = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.groupBoxBehavior = new System.Windows.Forms.GroupBox();
            this.checkBoxEnabled = new System.Windows.Forms.CheckBox();
            this.checkBoxMapAnalog = new System.Windows.Forms.CheckBox();
            this.checkBoxMapBinary = new System.Windows.Forms.CheckBox();
            this.listBoxHandlers = new System.Windows.Forms.ListBox();
            this.contextMenuStripHandlers = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.buttonAddBO = new System.Windows.Forms.Button();
            this.buttonAddAO = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.numericUpDownIndex = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxCode = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBoxBehavior.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownIndex)).BeginInit();
            this.SuspendLayout();
            // 
            // listBoxLog
            // 
            this.listBoxLog.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxLog.ContextMenuStrip = this.contextMenuStripOperations;
            this.listBoxLog.FormattingEnabled = true;
            this.listBoxLog.Location = new System.Drawing.Point(451, 39);
            this.listBoxLog.Name = "listBoxLog";
            this.listBoxLog.Size = new System.Drawing.Size(293, 238);
            this.listBoxLog.TabIndex = 2;
            // 
            // contextMenuStripOperations
            // 
            this.contextMenuStripOperations.Name = "contextMenuStripOperations";
            this.contextMenuStripOperations.Size = new System.Drawing.Size(153, 26);
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
            this.listBoxHandlers.ContextMenuStrip = this.contextMenuStripHandlers;
            this.listBoxHandlers.FormattingEnabled = true;
            this.listBoxHandlers.Location = new System.Drawing.Point(204, 39);
            this.listBoxHandlers.Name = "listBoxHandlers";
            this.listBoxHandlers.Size = new System.Drawing.Size(232, 121);
            this.listBoxHandlers.TabIndex = 11;
            // 
            // contextMenuStripHandlers
            // 
            this.contextMenuStripHandlers.Name = "contextMenuStripHandlers";
            this.contextMenuStripHandlers.Size = new System.Drawing.Size(61, 4);
            // 
            // buttonAddBO
            // 
            this.buttonAddBO.Location = new System.Drawing.Point(204, 238);
            this.buttonAddBO.Name = "buttonAddBO";
            this.buttonAddBO.Size = new System.Drawing.Size(109, 39);
            this.buttonAddBO.TabIndex = 12;
            this.buttonAddBO.Text = "Add CROB";
            this.buttonAddBO.UseVisualStyleBackColor = true;
            this.buttonAddBO.Click += new System.EventHandler(this.buttonAddBO_Click);
            // 
            // buttonAddAO
            // 
            this.buttonAddAO.Location = new System.Drawing.Point(327, 238);
            this.buttonAddAO.Name = "buttonAddAO";
            this.buttonAddAO.Size = new System.Drawing.Size(109, 39);
            this.buttonAddAO.TabIndex = 14;
            this.buttonAddAO.Text = "Add Analog";
            this.buttonAddAO.UseVisualStyleBackColor = true;
            this.buttonAddAO.Click += new System.EventHandler(this.buttonAddAO_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(343, 205);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 13);
            this.label2.TabIndex = 18;
            this.label2.Text = "Index";
            // 
            // numericUpDownIndex
            // 
            this.numericUpDownIndex.Location = new System.Drawing.Point(205, 203);
            this.numericUpDownIndex.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownIndex.Name = "numericUpDownIndex";
            this.numericUpDownIndex.Size = new System.Drawing.Size(120, 20);
            this.numericUpDownIndex.TabIndex = 17;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(343, 179);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(72, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "Return Status";
            // 
            // comboBoxCode
            // 
            this.comboBoxCode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxCode.FormattingEnabled = true;
            this.comboBoxCode.Location = new System.Drawing.Point(204, 175);
            this.comboBoxCode.Name = "comboBoxCode";
            this.comboBoxCode.Size = new System.Drawing.Size(121, 21);
            this.comboBoxCode.TabIndex = 15;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(201, 20);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(102, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Commands Handled";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(448, 20);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(184, 13);
            this.label4.TabIndex = 20;
            this.label4.Text = "Virtual Operations (any function code)";
            // 
            // CommandHandlerControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.numericUpDownIndex);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBoxCode);
            this.Controls.Add(this.buttonAddAO);
            this.Controls.Add(this.buttonAddBO);
            this.Controls.Add(this.listBoxHandlers);
            this.Controls.Add(this.groupBoxBehavior);
            this.Controls.Add(this.listBoxLog);
            this.Name = "CommandHandlerControl";
            this.Size = new System.Drawing.Size(766, 299);
            this.groupBoxBehavior.ResumeLayout(false);
            this.groupBoxBehavior.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownIndex)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox listBoxLog;
        private System.Windows.Forms.GroupBox groupBoxBehavior;
        private System.Windows.Forms.ListBox listBoxHandlers;
        private System.Windows.Forms.Button buttonAddBO;
        private System.Windows.Forms.CheckBox checkBoxMapAnalog;
        private System.Windows.Forms.CheckBox checkBoxMapBinary;
        private System.Windows.Forms.CheckBox checkBoxEnabled;
        private System.Windows.Forms.Button buttonAddAO;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numericUpDownIndex;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox comboBoxCode;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripHandlers;
        private System.Windows.Forms.ContextMenuStrip contextMenuStripOperations;
    }
}
