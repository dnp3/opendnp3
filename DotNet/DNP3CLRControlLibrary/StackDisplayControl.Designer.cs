namespace TestSetControlLibrary
{
    partial class StackDisplayControl
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
            this.tabControlStacks = new System.Windows.Forms.TabControl();
            this.SuspendLayout();
            // 
            // tabControlStacks
            // 
            this.tabControlStacks.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlStacks.Location = new System.Drawing.Point(0, 0);
            this.tabControlStacks.Name = "tabControlStacks";
            this.tabControlStacks.SelectedIndex = 0;
            this.tabControlStacks.Size = new System.Drawing.Size(1088, 549);
            this.tabControlStacks.TabIndex = 0;
            // 
            // StackDisplayControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tabControlStacks);
            this.Name = "StackDisplayControl";
            this.Size = new System.Drawing.Size(1088, 549);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlStacks;
    }
}
