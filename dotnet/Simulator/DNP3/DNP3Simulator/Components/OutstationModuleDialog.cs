using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.Simulator.DNP3.API;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class OutstationModuleDialog : Form
    {
        IOutstationModule selectedModule = null;

        public OutstationModuleDialog(IEnumerable<IOutstationModule> modules)
        {
            InitializeComponent();

            this.comboBoxModules.DataSource = modules.ToArray();
            this.SelectModule();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.SelectModule();
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        void SelectModule()
        {
            this.selectedModule = (IOutstationModule)this.comboBoxModules.SelectedItem;
            this.textBoxDesc.Text = this.selectedModule.Description;
        }

        public IOutstationModule SelectedModule
        {
            get
            {
                return selectedModule;
            }
        }

        private void comboBoxModules_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.SelectModule();
        }
    }
}
