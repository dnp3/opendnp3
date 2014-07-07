
namespace Automatak.Simulator.DNP3.Components
{
    partial class TemplateDialog
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
            this.groupBoxButtons = new System.Windows.Forms.GroupBox();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonSave = new System.Windows.Forms.Button();
            this.textBoxAlias = new System.Windows.Forms.TextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPageBinary = new System.Windows.Forms.TabPage();
            this.templateControlBinary = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.tabPageDoubleBinary = new System.Windows.Forms.TabPage();
            this.templateControlDoubleBinary = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.templateControlCounter = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.templateControlFrozenCounter = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.tabPageAnalog = new System.Windows.Forms.TabPage();
            this.templateControlAnalog = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.tabPageBinaryOutputStatus = new System.Windows.Forms.TabPage();
            this.templateControlBOStatus = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.AnalogOutputStatus = new System.Windows.Forms.TabPage();
            this.templateControlAOStatus = new Automatak.Simulator.DNP3.Components.TemplateControl();
            this.groupBoxButtons.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.tabControl.SuspendLayout();
            this.tabPageBinary.SuspendLayout();
            this.tabPageDoubleBinary.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPageAnalog.SuspendLayout();
            this.tabPageBinaryOutputStatus.SuspendLayout();
            this.AnalogOutputStatus.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxButtons
            // 
            this.groupBoxButtons.Controls.Add(this.label3);
            this.groupBoxButtons.Controls.Add(this.buttonSave);
            this.groupBoxButtons.Controls.Add(this.textBoxAlias);
            this.groupBoxButtons.Location = new System.Drawing.Point(12, 488);
            this.groupBoxButtons.Name = "groupBoxButtons";
            this.groupBoxButtons.Size = new System.Drawing.Size(610, 67);
            this.groupBoxButtons.TabIndex = 0;
            this.groupBoxButtons.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(110, 30);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(29, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "Alias";
            // 
            // buttonSave
            // 
            this.buttonSave.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonSave.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonSave.Location = new System.Drawing.Point(366, 15);
            this.buttonSave.Name = "buttonSave";
            this.buttonSave.Size = new System.Drawing.Size(238, 42);
            this.buttonSave.TabIndex = 0;
            this.buttonSave.Text = "Save";
            this.buttonSave.UseVisualStyleBackColor = true;
            this.buttonSave.Click += new System.EventHandler(this.buttonADD_Click);
            // 
            // textBoxAlias
            // 
            this.textBoxAlias.Location = new System.Drawing.Point(16, 27);
            this.textBoxAlias.Name = "textBoxAlias";
            this.textBoxAlias.Size = new System.Drawing.Size(88, 20);
            this.textBoxAlias.TabIndex = 4;
            this.textBoxAlias.Text = "template";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 561);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(634, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 5;
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            this.toolStripStatusLabel1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // tabControl
            // 
            this.tabControl.Alignment = System.Windows.Forms.TabAlignment.Right;
            this.tabControl.Controls.Add(this.tabPageBinary);
            this.tabControl.Controls.Add(this.tabPageDoubleBinary);
            this.tabControl.Controls.Add(this.tabPage2);
            this.tabControl.Controls.Add(this.tabPage3);
            this.tabControl.Controls.Add(this.tabPageAnalog);
            this.tabControl.Controls.Add(this.tabPageBinaryOutputStatus);
            this.tabControl.Controls.Add(this.AnalogOutputStatus);
            this.tabControl.Location = new System.Drawing.Point(12, 12);
            this.tabControl.Multiline = true;
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(610, 470);
            this.tabControl.TabIndex = 7;
            // 
            // tabPageBinary
            // 
            this.tabPageBinary.Controls.Add(this.templateControlBinary);
            this.tabPageBinary.Location = new System.Drawing.Point(4, 4);
            this.tabPageBinary.Name = "tabPageBinary";
            this.tabPageBinary.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBinary.Size = new System.Drawing.Size(583, 462);
            this.tabPageBinary.TabIndex = 0;
            this.tabPageBinary.Text = "Binary";
            this.tabPageBinary.UseVisualStyleBackColor = true;
            // 
            // templateControlBinary
            // 
            this.templateControlBinary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlBinary.Location = new System.Drawing.Point(3, 3);
            this.templateControlBinary.Name = "templateControlBinary";
            this.templateControlBinary.Size = new System.Drawing.Size(577, 456);
            this.templateControlBinary.TabIndex = 0;
            // 
            // tabPageDoubleBinary
            // 
            this.tabPageDoubleBinary.Controls.Add(this.templateControlDoubleBinary);
            this.tabPageDoubleBinary.Location = new System.Drawing.Point(4, 4);
            this.tabPageDoubleBinary.Name = "tabPageDoubleBinary";
            this.tabPageDoubleBinary.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageDoubleBinary.Size = new System.Drawing.Size(583, 462);
            this.tabPageDoubleBinary.TabIndex = 1;
            this.tabPageDoubleBinary.Text = "Double Binary";
            this.tabPageDoubleBinary.UseVisualStyleBackColor = true;
            // 
            // templateControlDoubleBinary
            // 
            this.templateControlDoubleBinary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlDoubleBinary.Location = new System.Drawing.Point(3, 3);
            this.templateControlDoubleBinary.Name = "templateControlDoubleBinary";
            this.templateControlDoubleBinary.Size = new System.Drawing.Size(577, 456);
            this.templateControlDoubleBinary.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.templateControlCounter);
            this.tabPage2.Location = new System.Drawing.Point(4, 4);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(583, 462);
            this.tabPage2.TabIndex = 2;
            this.tabPage2.Text = "Counter";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // templateControlCounter
            // 
            this.templateControlCounter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlCounter.Location = new System.Drawing.Point(3, 3);
            this.templateControlCounter.Name = "templateControlCounter";
            this.templateControlCounter.Size = new System.Drawing.Size(577, 456);
            this.templateControlCounter.TabIndex = 1;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.templateControlFrozenCounter);
            this.tabPage3.Location = new System.Drawing.Point(4, 4);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(583, 462);
            this.tabPage3.TabIndex = 3;
            this.tabPage3.Text = "Frozen Counter";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // templateControlFrozenCounter
            // 
            this.templateControlFrozenCounter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlFrozenCounter.Location = new System.Drawing.Point(3, 3);
            this.templateControlFrozenCounter.Name = "templateControlFrozenCounter";
            this.templateControlFrozenCounter.Size = new System.Drawing.Size(577, 456);
            this.templateControlFrozenCounter.TabIndex = 1;
            // 
            // tabPageAnalog
            // 
            this.tabPageAnalog.Controls.Add(this.templateControlAnalog);
            this.tabPageAnalog.Location = new System.Drawing.Point(4, 4);
            this.tabPageAnalog.Name = "tabPageAnalog";
            this.tabPageAnalog.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageAnalog.Size = new System.Drawing.Size(583, 462);
            this.tabPageAnalog.TabIndex = 4;
            this.tabPageAnalog.Text = "Analog";
            this.tabPageAnalog.UseVisualStyleBackColor = true;
            // 
            // templateControlAnalog
            // 
            this.templateControlAnalog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlAnalog.Location = new System.Drawing.Point(3, 3);
            this.templateControlAnalog.Name = "templateControlAnalog";
            this.templateControlAnalog.Size = new System.Drawing.Size(577, 456);
            this.templateControlAnalog.TabIndex = 1;
            // 
            // tabPageBinaryOutputStatus
            // 
            this.tabPageBinaryOutputStatus.Controls.Add(this.templateControlBOStatus);
            this.tabPageBinaryOutputStatus.Location = new System.Drawing.Point(4, 4);
            this.tabPageBinaryOutputStatus.Name = "tabPageBinaryOutputStatus";
            this.tabPageBinaryOutputStatus.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBinaryOutputStatus.Size = new System.Drawing.Size(583, 462);
            this.tabPageBinaryOutputStatus.TabIndex = 5;
            this.tabPageBinaryOutputStatus.Text = "BO Status";
            this.tabPageBinaryOutputStatus.UseVisualStyleBackColor = true;
            // 
            // templateControlBOStatus
            // 
            this.templateControlBOStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlBOStatus.Location = new System.Drawing.Point(3, 3);
            this.templateControlBOStatus.Name = "templateControlBOStatus";
            this.templateControlBOStatus.Size = new System.Drawing.Size(577, 456);
            this.templateControlBOStatus.TabIndex = 1;
            // 
            // AnalogOutputStatus
            // 
            this.AnalogOutputStatus.Controls.Add(this.templateControlAOStatus);
            this.AnalogOutputStatus.Location = new System.Drawing.Point(4, 4);
            this.AnalogOutputStatus.Name = "AnalogOutputStatus";
            this.AnalogOutputStatus.Padding = new System.Windows.Forms.Padding(3);
            this.AnalogOutputStatus.Size = new System.Drawing.Size(583, 462);
            this.AnalogOutputStatus.TabIndex = 6;
            this.AnalogOutputStatus.Text = "AO Status";
            this.AnalogOutputStatus.UseVisualStyleBackColor = true;
            // 
            // templateControlAOStatus
            // 
            this.templateControlAOStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.templateControlAOStatus.Location = new System.Drawing.Point(3, 3);
            this.templateControlAOStatus.Name = "templateControlAOStatus";
            this.templateControlAOStatus.Size = new System.Drawing.Size(577, 456);
            this.templateControlAOStatus.TabIndex = 1;
            // 
            // TemplateDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(634, 583);
            this.Controls.Add(this.tabControl);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.groupBoxButtons);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "TemplateDialog";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Edit Template";
            this.TopMost = true;            
            this.groupBoxButtons.ResumeLayout(false);
            this.groupBoxButtons.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.tabControl.ResumeLayout(false);
            this.tabPageBinary.ResumeLayout(false);
            this.tabPageDoubleBinary.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPageAnalog.ResumeLayout(false);
            this.tabPageBinaryOutputStatus.ResumeLayout(false);
            this.AnalogOutputStatus.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxButtons;
        private System.Windows.Forms.Button buttonSave;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPageBinary;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxAlias;
        private System.Windows.Forms.TabPage tabPageDoubleBinary;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPageAnalog;
        private System.Windows.Forms.TabPage tabPageBinaryOutputStatus;
        private System.Windows.Forms.TabPage AnalogOutputStatus;
        private TemplateControl templateControlBinary;
        private TemplateControl templateControlDoubleBinary;
        private TemplateControl templateControlCounter;
        private TemplateControl templateControlFrozenCounter;
        private TemplateControl templateControlAnalog;
        private TemplateControl templateControlBOStatus;
        private TemplateControl templateControlAOStatus;
    }
}