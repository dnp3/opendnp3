using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class StaticResponseTypeControl : ResponseTypeControl
    {
        public StaticResponseTypeControl()
        {
            InitializeComponent();

            this.ComboBoxBinary.DataSource = Enum.GetNames(typeof(StaticBinaryResponse));
            this.ComboBoxDoubleBinary.DataSource = Enum.GetNames(typeof(StaticDoubleBinaryResponse));
            this.ComboBoxCounter.DataSource = Enum.GetNames(typeof(StaticCounterResponse));
            this.ComboBoxFrozenCounter.DataSource = Enum.GetNames(typeof(StaticFrozenCounterResponse));
            this.ComboBoxAnalog.DataSource = Enum.GetNames(typeof(StaticAnalogResponse));
            this.ComboBoxBinaryOutputStatus.DataSource = Enum.GetNames(typeof(StaticBinaryOutputStatusResponse));
            this.ComboBoxAnalogOutputStatus.DataSource = Enum.GetNames(typeof(StaticAnalogOutputStatusResponse));
        }

        public StaticResponseConfig Configuration
        {
            get
            {
                var config = new StaticResponseConfig();

                config.binary = (StaticBinaryResponse) this.ComboBoxBinary.SelectedValue;
                config.doubleBinary = (StaticDoubleBinaryResponse) this.ComboBoxDoubleBinary.SelectedValue;
                config.counter = (StaticCounterResponse) this.ComboBoxCounter.SelectedValue;
                config.frozenCounter = (StaticFrozenCounterResponse) this.ComboBoxFrozenCounter.SelectedValue;
                config.analog = (StaticAnalogResponse)this.ComboBoxAnalog.SelectedValue;
                config.binaryOutputStatus = (StaticBinaryOutputStatusResponse) this.ComboBoxBinaryOutputStatus.SelectedValue;
                config.analogOutputStatus = (StaticAnalogOutputStatusResponse) this.ComboBoxAnalogOutputStatus.SelectedValue;

                return config;
            }
        }
    }
}
