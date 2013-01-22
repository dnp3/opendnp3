
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
namespace TestSetControlLibrary
{
    partial class SetpointForm
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
            this.groupBoxParameters = new System.Windows.Forms.GroupBox();
            this.buttonSelectOperate = new System.Windows.Forms.Button();
            this.buttonDirectOperate = new System.Windows.Forms.Button();
            this.numericUpDownIndex = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxType = new System.Windows.Forms.ComboBox();
            this.labelValue = new System.Windows.Forms.Label();
            this.numericUpDownValue = new System.Windows.Forms.NumericUpDown();
            this.textBoxStatus = new System.Windows.Forms.TextBox();
            this.groupBoxParameters.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownIndex)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownValue)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBoxParameters
            // 
            this.groupBoxParameters.Controls.Add(this.buttonSelectOperate);
            this.groupBoxParameters.Controls.Add(this.buttonDirectOperate);
            this.groupBoxParameters.Controls.Add(this.numericUpDownIndex);
            this.groupBoxParameters.Controls.Add(this.label2);
            this.groupBoxParameters.Controls.Add(this.label1);
            this.groupBoxParameters.Controls.Add(this.comboBoxType);
            this.groupBoxParameters.Controls.Add(this.labelValue);
            this.groupBoxParameters.Controls.Add(this.numericUpDownValue);
            this.groupBoxParameters.Location = new System.Drawing.Point(34, 15);
            this.groupBoxParameters.Name = "groupBoxParameters";
            this.groupBoxParameters.Size = new System.Drawing.Size(248, 235);
            this.groupBoxParameters.TabIndex = 0;
            this.groupBoxParameters.TabStop = false;
            this.groupBoxParameters.Text = "Parameters";
            // 
            // buttonSelectOperate
            // 
            this.buttonSelectOperate.Location = new System.Drawing.Point(38, 155);
            this.buttonSelectOperate.Name = "buttonSelectOperate";
            this.buttonSelectOperate.Size = new System.Drawing.Size(177, 27);
            this.buttonSelectOperate.TabIndex = 9;
            this.buttonSelectOperate.Text = "Select and Operate";
            this.buttonSelectOperate.UseVisualStyleBackColor = true;
            this.buttonSelectOperate.Click += new System.EventHandler(this.buttonSelectOperate_Click);
            // 
            // buttonDirectOperate
            // 
            this.buttonDirectOperate.Location = new System.Drawing.Point(38, 188);
            this.buttonDirectOperate.Name = "buttonDirectOperate";
            this.buttonDirectOperate.Size = new System.Drawing.Size(177, 27);
            this.buttonDirectOperate.TabIndex = 8;
            this.buttonDirectOperate.Text = "Direct Operate";
            this.buttonDirectOperate.UseVisualStyleBackColor = true;
            this.buttonDirectOperate.Click += new System.EventHandler(this.buttonDirectOperate_Click);
            // 
            // numericUpDownIndex
            // 
            this.numericUpDownIndex.Location = new System.Drawing.Point(116, 108);
            this.numericUpDownIndex.Maximum = new decimal(new int[] {
            1410065408,
            2,
            0,
            0});
            this.numericUpDownIndex.Name = "numericUpDownIndex";
            this.numericUpDownIndex.Size = new System.Drawing.Size(99, 20);
            this.numericUpDownIndex.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(38, 110);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(33, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Index";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(38, 73);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Type";
            // 
            // comboBoxType
            // 
            this.comboBoxType.FormattingEnabled = true;
            this.comboBoxType.Items.AddRange(new object[] {
            "16-bit Integer",
            "32-bit Integer",
            "32-bit Float",
            "64-bit Double"});
            this.comboBoxType.Location = new System.Drawing.Point(116, 70);
            this.comboBoxType.Name = "comboBoxType";
            this.comboBoxType.Size = new System.Drawing.Size(99, 21);
            this.comboBoxType.TabIndex = 2;
            this.comboBoxType.Text = "32-bit Integer";
            // 
            // labelValue
            // 
            this.labelValue.AutoSize = true;
            this.labelValue.Location = new System.Drawing.Point(35, 31);
            this.labelValue.Name = "labelValue";
            this.labelValue.Size = new System.Drawing.Size(34, 13);
            this.labelValue.TabIndex = 1;
            this.labelValue.Text = "Value";
            // 
            // numericUpDownValue
            // 
            this.numericUpDownValue.DecimalPlaces = 2;
            this.numericUpDownValue.Location = new System.Drawing.Point(116, 31);
            this.numericUpDownValue.Maximum = new decimal(new int[] {
            1000000,
            0,
            0,
            0});
            this.numericUpDownValue.Minimum = new decimal(new int[] {
            1000000,
            0,
            0,
            -2147483648});
            this.numericUpDownValue.Name = "numericUpDownValue";
            this.numericUpDownValue.Size = new System.Drawing.Size(99, 20);
            this.numericUpDownValue.TabIndex = 0;
            // 
            // textBoxStatus
            // 
            this.textBoxStatus.Location = new System.Drawing.Point(34, 256);
            this.textBoxStatus.Name = "textBoxStatus";
            this.textBoxStatus.ReadOnly = true;
            this.textBoxStatus.Size = new System.Drawing.Size(248, 20);
            this.textBoxStatus.TabIndex = 1;
            // 
            // SetpointForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(299, 289);
            this.Controls.Add(this.textBoxStatus);
            this.Controls.Add(this.groupBoxParameters);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SetpointForm";
            this.ShowIcon = false;
            this.Text = "Issue Setpoint";
            this.TopMost = true;
            this.groupBoxParameters.ResumeLayout(false);
            this.groupBoxParameters.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownIndex)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownValue)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxParameters;
        private System.Windows.Forms.ComboBox comboBoxType;
        private System.Windows.Forms.Label labelValue;
        private System.Windows.Forms.NumericUpDown numericUpDownValue;
        private System.Windows.Forms.NumericUpDown numericUpDownIndex;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxStatus;
        private System.Windows.Forms.Button buttonDirectOperate;
        private System.Windows.Forms.Button buttonSelectOperate;
    }
}