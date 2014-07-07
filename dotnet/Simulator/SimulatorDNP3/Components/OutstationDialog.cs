using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    partial class OutstationDialog : Form
    {
        readonly IDNP3Config config;

        public OutstationDialog(IDNP3Config config)
        {
            InitializeComponent();

            this.config = config;
            this.linkConfigControl.IsMaster = false;

            comboBoxTemplate.DataSource = config.Templates.Select(kv => kv.Key).ToList();            
        }

        private void buttonADD_Click(object sender, EventArgs e)
        {            
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public String SelectedAlias
        {
            get
            {
                return textBoxID.Text;
            }
        }

        OutstationParams OutstationParameters
        {
            get
            {
                OutstationParams ret = new OutstationParams();
                ret.allowUnsolicited = this.checkBoxEnableUnsol.Checked;
                ret.maxControlsPerRequest = Decimal.ToByte(this.numericUpDownMaxControls.Value);
                ret.maxTxFragSize = Decimal.ToUInt16(this.numericUpDownMaxTxFrag.Value);
                ret.selectTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownSelectTimeout.Value));
                ret.unsolicitedConfirmTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownUnsolConfirmTimeout.Value));
                ret.solicitedConfirmTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownSolConfirmTimeout.Value));
                ret.unsolicitedRetryPeriod = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownUnsolicitedRetry.Value));
                return ret;
            }
        }

        public OutstationStackConfig Configuration
        {
            get
            {
                var oc = new OutstationStackConfig();
                oc.link = this.linkConfigControl.Configuration;
                oc.outstation.config = this.OutstationParameters;
                oc.buffer = this.eventBufferConfigControl1.Configuration;
                oc.defaultStaticResponses = this.staticResponseTypeControl1.Configuration;
                oc.defaultEventResponses = this.eventResponseTypeControl1.Configuration;
            
                var alias = this.comboBoxTemplate.SelectedItem.ToString();
                var template = config.GetTemplateMaybeNull(alias);
                oc.databaseTemplate = template;
                return oc;
            }
        }

        void CheckState()
        {
            if (comboBoxTemplate.SelectedValue == null)
            {
                this.buttonEdit.Enabled = false;
                this.buttonAdd.Enabled = false;
                this.toolStripStatusLabel1.Text = "You must select a device template";
            }
            else
            {
                this.buttonEdit.Enabled = true;
                this.buttonAdd.Enabled = true;
                this.toolStripStatusLabel1.Text = "";
            }
        }

        void OutstationDialog_Load(object sender, EventArgs e)
        {
            CheckState();
        }

        private void buttonNew_Click(object sender, EventArgs e)
        {
            using (var dialog = new TemplateDialog("template1", new DatabaseTemplate()))
            {
                dialog.ShowDialog();
                if (dialog.DialogResult == DialogResult.OK)
                {
                    var template = dialog.ConfiguredTemplate;
                    config.AddTemplate(dialog.SelectedAlias, template);
                    this.comboBoxTemplate.DataSource = config.Templates.Select(kvp => kvp.Key).ToList();                    
                }
            }
        }       

        private void comboBoxTemplate_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.CheckState();
        }

        private void buttonEdit_Click(object sender, EventArgs e)
        {
            var alias = this.comboBoxTemplate.SelectedItem.ToString();
            var edited = config.GetTemplateMaybeNull(alias);

            if (edited != null)
            {
                using (var dialog = new TemplateDialog(alias, edited))
                {
                    dialog.ShowDialog();
                    if (dialog.DialogResult == DialogResult.OK)
                    {
                        var template = dialog.ConfiguredTemplate;
                        config.AddTemplate(dialog.SelectedAlias, template);
                        this.comboBoxTemplate.DataSource = config.Templates.Select(kvp => kvp.Key).ToList();
                    }
                }
            }
           
        }
    }
}
