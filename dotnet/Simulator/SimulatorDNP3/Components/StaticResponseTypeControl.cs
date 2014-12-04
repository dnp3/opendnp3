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
    public partial class StaticResponseTypeControl : ResponseTypeControl
    {
        public StaticResponseTypeControl()
        {
            InitializeComponent();

            this.ComboBoxBinary.DataSource = Enum.GetValues(typeof(StaticBinaryVariation));
            this.ComboBoxDoubleBinary.DataSource = Enum.GetValues(typeof(StaticDoubleBinaryVariation));
            this.ComboBoxCounter.DataSource = Enum.GetValues(typeof(StaticCounterVariation));
            this.ComboBoxFrozenCounter.DataSource = Enum.GetValues(typeof(StaticFrozenCounterVariation));
            this.ComboBoxAnalog.DataSource = Enum.GetValues(typeof(StaticAnalogVariation));
            this.ComboBoxBinaryOutputStatus.DataSource = Enum.GetValues(typeof(StaticBinaryOutputStatusVariation));
            this.ComboBoxAnalogOutputStatus.DataSource = Enum.GetValues(typeof(StaticAnalogOutputStatusVariation));
        }

        public StaticResponseConfig Configuration
        {
            get
            {
                var config = new StaticResponseConfig();

                config.binary = (StaticBinaryVariation) this.ComboBoxBinary.SelectedValue;
                config.doubleBinary = (StaticDoubleBinaryVariation) this.ComboBoxDoubleBinary.SelectedValue;
                config.counter = (StaticCounterVariation) this.ComboBoxCounter.SelectedValue;
                config.frozenCounter = (StaticFrozenCounterVariation) this.ComboBoxFrozenCounter.SelectedValue;
                config.analog = (StaticAnalogVariation)this.ComboBoxAnalog.SelectedValue;
                config.binaryOutputStatus = (StaticBinaryOutputStatusVariation) this.ComboBoxBinaryOutputStatus.SelectedValue;
                config.analogOutputStatus = (StaticAnalogOutputStatusVariation) this.ComboBoxAnalogOutputStatus.SelectedValue;

                return config;
            }
        }
    }
}
