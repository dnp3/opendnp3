using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class MasterConfigControl : UserControl
    {
        public MasterConfigControl()
        {
            InitializeComponent();

            this.comboBoxTimeSync.DataSource = Enum.GetValues(typeof(TimeSyncMode));
            this.comboBoxTimeSync.SelectedItem = TimeSyncMode.None;
            
            this.classFieldControlIntegrity.ClassFieldValue = ClassField.AllClasses;
            this.classFieldControlEnableUnsol.ClassFieldValue = ClassField.AllEventClasses;
            this.classFieldControlEnableUnsol.AllowClass0 = false;
        }

        public MasterConfig Configuration
        {
            get
            {
                var config = new MasterConfig();
                config.disableUnsolOnStartup = this.checkBoxDisableUnsolicited.Checked;
                config.startupIntegrityClassMask = this.classFieldControlIntegrity.ClassFieldValue;
                config.unsolClassMask = this.classFieldControlEnableUnsol.ClassFieldValue;
                config.timeSyncMode = (TimeSyncMode)this.comboBoxTimeSync.SelectedItem;
                config.integrityOnEventOverflowIIN = this.checkBoxIntegrityOnOverflow.Checked;
                config.responseTimeout = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownResponseTimeout.Value));
                config.taskRetryPeriod = TimeSpan.FromMilliseconds(Decimal.ToDouble(this.numericUpDownTaskRetryDelay.Value));
                return config;
            }
        }
    }
}
