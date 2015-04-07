namespace Automatak.Simulator.DNP3.RelayOutstationPlugin
{
    partial class OutstationForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OutstationForm));
            this.pictureBoxRelay = new System.Windows.Forms.PictureBox();
            this.transparentButtonTrip = new Automatak.Simulator.DNP3.RelayOutstationPlugin.TransparentButton();
            this.transparentButtonClose = new Automatak.Simulator.DNP3.RelayOutstationPlugin.TransparentButton();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxRelay)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxRelay
            // 
            this.pictureBoxRelay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxRelay.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxRelay.Image")));
            this.pictureBoxRelay.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxRelay.Name = "pictureBoxRelay";
            this.pictureBoxRelay.Size = new System.Drawing.Size(597, 311);
            this.pictureBoxRelay.TabIndex = 0;
            this.pictureBoxRelay.TabStop = false;
            // 
            // transparentButtonTrip
            // 
            this.transparentButtonTrip.BackColor = System.Drawing.Color.Transparent;
            this.transparentButtonTrip.ForeColor = System.Drawing.Color.Transparent;
            this.transparentButtonTrip.Location = new System.Drawing.Point(466, 168);
            this.transparentButtonTrip.Name = "transparentButtonTrip";
            this.transparentButtonTrip.Size = new System.Drawing.Size(34, 32);
            this.transparentButtonTrip.TabIndex = 1;
            this.transparentButtonTrip.Click += new System.EventHandler(this.transparentButtonTrip_Click);
            // 
            // transparentButtonClose
            // 
            this.transparentButtonClose.BackColor = System.Drawing.Color.Transparent;
            this.transparentButtonClose.ForeColor = System.Drawing.Color.Transparent;
            this.transparentButtonClose.Location = new System.Drawing.Point(466, 127);
            this.transparentButtonClose.Name = "transparentButtonClose";
            this.transparentButtonClose.Size = new System.Drawing.Size(34, 32);
            this.transparentButtonClose.TabIndex = 2;
            this.transparentButtonClose.Click += new System.EventHandler(this.transparentButtonClose_Click);
            // 
            // OutstationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(597, 311);
            this.Controls.Add(this.transparentButtonClose);
            this.Controls.Add(this.transparentButtonTrip);
            this.Controls.Add(this.pictureBoxRelay);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OutstationForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Relay Simulator";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxRelay)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxRelay;
        private TransparentButton transparentButtonTrip;
        private TransparentButton transparentButtonClose;
    }
}