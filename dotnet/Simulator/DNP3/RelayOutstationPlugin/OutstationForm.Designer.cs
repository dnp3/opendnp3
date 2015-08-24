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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OutstationForm));
            this.pictureBoxRelay = new System.Windows.Forms.PictureBox();
            this.labelValueIA = new System.Windows.Forms.Label();
            this.labelValueIB = new System.Windows.Forms.Label();
            this.labelValueIC = new System.Windows.Forms.Label();
            this.labelValueVC = new System.Windows.Forms.Label();
            this.labelValueVB = new System.Windows.Forms.Label();
            this.labelValueVA = new System.Windows.Forms.Label();
            this.timerUpdate = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxRelay)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxRelay
            // 
            this.pictureBoxRelay.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxRelay.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxRelay.Image")));
            this.pictureBoxRelay.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxRelay.Name = "pictureBoxRelay";
            this.pictureBoxRelay.Size = new System.Drawing.Size(613, 396);
            this.pictureBoxRelay.TabIndex = 0;
            this.pictureBoxRelay.TabStop = false;
            this.pictureBoxRelay.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pictureBoxRelay_MouseClick);
            this.pictureBoxRelay.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBoxRelay_MouseMove);
            // 
            // labelValueIA
            // 
            this.labelValueIA.AutoSize = true;
            this.labelValueIA.Location = new System.Drawing.Point(54, 283);
            this.labelValueIA.Name = "labelValueIA";
            this.labelValueIA.Size = new System.Drawing.Size(58, 13);
            this.labelValueIA.TabIndex = 3;
            this.labelValueIA.Text = "<value IA>";
            // 
            // labelValueIB
            // 
            this.labelValueIB.AutoSize = true;
            this.labelValueIB.Location = new System.Drawing.Point(54, 313);
            this.labelValueIB.Name = "labelValueIB";
            this.labelValueIB.Size = new System.Drawing.Size(58, 13);
            this.labelValueIB.TabIndex = 4;
            this.labelValueIB.Text = "<value IB>";
            // 
            // labelValueIC
            // 
            this.labelValueIC.AutoSize = true;
            this.labelValueIC.Location = new System.Drawing.Point(54, 345);
            this.labelValueIC.Name = "labelValueIC";
            this.labelValueIC.Size = new System.Drawing.Size(58, 13);
            this.labelValueIC.TabIndex = 5;
            this.labelValueIC.Text = "<value IC>";
            // 
            // labelValueVC
            // 
            this.labelValueVC.AutoSize = true;
            this.labelValueVC.Location = new System.Drawing.Point(159, 345);
            this.labelValueVC.Name = "labelValueVC";
            this.labelValueVC.Size = new System.Drawing.Size(62, 13);
            this.labelValueVC.TabIndex = 8;
            this.labelValueVC.Text = "<value VC>";
            // 
            // labelValueVB
            // 
            this.labelValueVB.AutoSize = true;
            this.labelValueVB.Location = new System.Drawing.Point(159, 313);
            this.labelValueVB.Name = "labelValueVB";
            this.labelValueVB.Size = new System.Drawing.Size(62, 13);
            this.labelValueVB.TabIndex = 7;
            this.labelValueVB.Text = "<value VB>";
            // 
            // labelValueVA
            // 
            this.labelValueVA.AutoSize = true;
            this.labelValueVA.Location = new System.Drawing.Point(159, 283);
            this.labelValueVA.Name = "labelValueVA";
            this.labelValueVA.Size = new System.Drawing.Size(62, 13);
            this.labelValueVA.TabIndex = 6;
            this.labelValueVA.Text = "<value VA>";
            // 
            // timerUpdate
            // 
            this.timerUpdate.Enabled = true;
            this.timerUpdate.Interval = 1000;
            this.timerUpdate.Tick += new System.EventHandler(this.timerUpdate_Tick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(277, 275);
            this.label1.MaximumSize = new System.Drawing.Size(275, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(271, 90);
            this.label1.TabIndex = 9;
            this.label1.Text = "A very simple relay simulation. It reports 3-phase voltage and current, and respo" +
    "nds to a CROB latch on/off on index 0.  The breaker can be manually operated via" +
    " the buttons on the relay.";
            // 
            // OutstationForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(613, 396);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.labelValueVC);
            this.Controls.Add(this.labelValueVB);
            this.Controls.Add(this.labelValueVA);
            this.Controls.Add(this.labelValueIC);
            this.Controls.Add(this.labelValueIB);
            this.Controls.Add(this.labelValueIA);
            this.Controls.Add(this.pictureBoxRelay);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OutstationForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Relay Simulator";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OutstationForm_FormClosing);
            this.Load += new System.EventHandler(this.OutstationForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxRelay)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxRelay;
        private System.Windows.Forms.Label labelValueIA;
        private System.Windows.Forms.Label labelValueIB;
        private System.Windows.Forms.Label labelValueIC;
        private System.Windows.Forms.Label labelValueVC;
        private System.Windows.Forms.Label labelValueVB;
        private System.Windows.Forms.Label labelValueVA;
        private System.Windows.Forms.Timer timerUpdate;
        private System.Windows.Forms.Label label1;
    }
}