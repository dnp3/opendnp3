namespace Automatak.Simulator.UI
{
    partial class LogFileControl
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LogFileControl));
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.buttonPlay = new System.Windows.Forms.Button();
            this.buttonPause = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.buttonBrowse.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonBrowse.ImageIndex = 0;
            this.buttonBrowse.ImageList = this.imageList;
            this.buttonBrowse.Location = new System.Drawing.Point(0, 0);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(42, 38);
            this.buttonBrowse.TabIndex = 0;
            this.buttonBrowse.UseVisualStyleBackColor = true;
            // 
            // imageList
            // 
            this.imageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList.ImageStream")));
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList.Images.SetKeyName(0, "folder.png");
            this.imageList.Images.SetKeyName(1, "pause.png");
            this.imageList.Images.SetKeyName(2, "play.png");
            // 
            // buttonPlay
            // 
            this.buttonPlay.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonPlay.ImageIndex = 2;
            this.buttonPlay.ImageList = this.imageList;
            this.buttonPlay.Location = new System.Drawing.Point(43, 0);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(42, 38);
            this.buttonPlay.TabIndex = 2;
            this.buttonPlay.UseVisualStyleBackColor = true;
            // 
            // buttonPause
            // 
            this.buttonPause.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.buttonPause.ImageIndex = 1;
            this.buttonPause.ImageList = this.imageList;
            this.buttonPause.Location = new System.Drawing.Point(86, 0);
            this.buttonPause.Name = "buttonPause";
            this.buttonPause.Size = new System.Drawing.Size(42, 38);
            this.buttonPause.TabIndex = 3;
            this.buttonPause.UseVisualStyleBackColor = true;
            // 
            // LogFileControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Controls.Add(this.buttonPause);
            this.Controls.Add(this.buttonPlay);
            this.Controls.Add(this.buttonBrowse);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "LogFileControl";
            this.Size = new System.Drawing.Size(130, 39);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.Button buttonPlay;
        private System.Windows.Forms.Button buttonPause;
    }
}
