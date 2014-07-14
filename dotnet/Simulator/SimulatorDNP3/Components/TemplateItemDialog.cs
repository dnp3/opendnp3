using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;


using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class TemplateItemDialog : Form
    {
        public TemplateItemDialog()
        {
            InitializeComponent();

            this.comboBoxPointClass.DataSource = Enum.GetValues(typeof(PointClass));
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public PointClass SelectedPointClass
        {
            get
            {
                return (PointClass) comboBoxPointClass.SelectedItem;
            }
        }
    }
}
