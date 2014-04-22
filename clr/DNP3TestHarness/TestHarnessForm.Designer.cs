namespace DNP3TestHarness
{
    partial class TestHarnessForm
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
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.listBoxLog = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(0, 0);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(177, 555);
            this.splitter1.TabIndex = 0;
            this.splitter1.TabStop = false;
            // 
            // listBoxLog
            // 
            this.listBoxLog.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxLog.Font = new System.Drawing.Font("Calibri", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.listBoxLog.FormattingEnabled = true;
            this.listBoxLog.HorizontalScrollbar = true;
            this.listBoxLog.ItemHeight = 18;
            this.listBoxLog.Location = new System.Drawing.Point(177, 0);
            this.listBoxLog.Name = "listBoxLog";
            this.listBoxLog.ScrollAlwaysVisible = true;
            this.listBoxLog.Size = new System.Drawing.Size(824, 555);
            this.listBoxLog.TabIndex = 1;
            // 
            // TestHarnessForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1001, 555);
            this.Controls.Add(this.listBoxLog);
            this.Controls.Add(this.splitter1);
            this.DoubleBuffered = true;
            this.Enabled = false;
            this.Name = "TestHarnessForm";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.ListBox listBoxLog;
    }
}

