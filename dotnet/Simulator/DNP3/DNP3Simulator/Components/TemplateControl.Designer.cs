namespace Automatak.Simulator.DNP3.Components
{
    partial class TemplateControl
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
            this.listViewMeas = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.numericUpDownCount = new System.Windows.Forms.NumericUpDown();
            this.buttonEdit = new System.Windows.Forms.Button();
            this.buttonClear = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownCount)).BeginInit();
            this.SuspendLayout();
            // 
            // listViewMeas
            // 
            this.listViewMeas.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listViewMeas.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.listViewMeas.FullRowSelect = true;
            this.listViewMeas.HideSelection = false;
            this.listViewMeas.Location = new System.Drawing.Point(0, 0);
            this.listViewMeas.Name = "listViewMeas";
            this.listViewMeas.Size = new System.Drawing.Size(722, 440);
            this.listViewMeas.TabIndex = 0;
            this.listViewMeas.UseCompatibleStateImageBehavior = false;
            this.listViewMeas.View = System.Windows.Forms.View.Details;
            this.listViewMeas.SelectedIndexChanged += new System.EventHandler(this.listViewMeas_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Index";
            this.columnHeader1.Width = 142;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Class Assignment";
            this.columnHeader2.Width = 144;
            // 
            // numericUpDownCount
            // 
            this.numericUpDownCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.numericUpDownCount.Location = new System.Drawing.Point(11, 455);
            this.numericUpDownCount.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numericUpDownCount.Name = "numericUpDownCount";
            this.numericUpDownCount.Size = new System.Drawing.Size(109, 20);
            this.numericUpDownCount.TabIndex = 1;
            this.numericUpDownCount.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDownCount.ValueChanged += new System.EventHandler(this.numericUpDownCount_ValueChanged);
            // 
            // buttonEdit
            // 
            this.buttonEdit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonEdit.Location = new System.Drawing.Point(572, 451);
            this.buttonEdit.Name = "buttonEdit";
            this.buttonEdit.Size = new System.Drawing.Size(136, 27);
            this.buttonEdit.TabIndex = 2;
            this.buttonEdit.Text = "Edit Selected Rows";
            this.buttonEdit.UseVisualStyleBackColor = true;
            this.buttonEdit.Click += new System.EventHandler(this.buttonEdit_Click);
            // 
            // buttonClear
            // 
            this.buttonClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClear.Location = new System.Drawing.Point(430, 451);
            this.buttonClear.Name = "buttonClear";
            this.buttonClear.Size = new System.Drawing.Size(136, 27);
            this.buttonClear.TabIndex = 3;
            this.buttonClear.Text = "Clear";
            this.buttonClear.UseVisualStyleBackColor = true;
            this.buttonClear.Click += new System.EventHandler(this.buttonClear_Click);
            // 
            // TemplateControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.buttonClear);
            this.Controls.Add(this.buttonEdit);
            this.Controls.Add(this.numericUpDownCount);
            this.Controls.Add(this.listViewMeas);
            this.Name = "TemplateControl";
            this.Size = new System.Drawing.Size(722, 494);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownCount)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listViewMeas;
        private System.Windows.Forms.NumericUpDown numericUpDownCount;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button buttonEdit;
        private System.Windows.Forms.Button buttonClear;
    }
}
