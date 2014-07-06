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
    public partial class EventResponseTypeControl : ResponseTypeControl
    {
        public EventResponseTypeControl()
        {
            InitializeComponent();

            this.ComboBoxBinary.DataSource = Enum.GetNames(typeof(EventBinaryResponse));
            this.ComboBoxDoubleBinary.DataSource = Enum.GetNames(typeof(EventDoubleBinaryResponse));
            this.ComboBoxCounter.DataSource = Enum.GetNames(typeof(EventCounterResponse));
            this.ComboBoxFrozenCounter.DataSource = Enum.GetNames(typeof(EventFrozenCounterResponse));
            this.ComboBoxAnalog.DataSource = Enum.GetNames(typeof(EventAnalogResponse));
            this.ComboBoxBinaryOutputStatus.DataSource = Enum.GetNames(typeof(EventBinaryOutputStatusResponse));
            this.ComboBoxAnalogOutputStatus.DataSource = Enum.GetNames(typeof(EventAnalogOutputStatusResponse));
        }

        public EventResponseConfig Configuration
        {
            get
            {
                var config = new EventResponseConfig();
                
                config.binary = (EventBinaryResponse) this.ComboBoxBinary.SelectedValue;
                config.doubleBinary = (EventDoubleBinaryResponse) this.ComboBoxDoubleBinary.SelectedValue;
                config.counter = (EventCounterResponse) this.ComboBoxCounter.SelectedValue;
                config.frozenCounter = (EventFrozenCounterResponse) this.ComboBoxFrozenCounter.SelectedValue;
                config.analog = (EventAnalogResponse) this.ComboBoxAnalog.SelectedValue;
                config.binaryOutputStatus = (EventBinaryOutputStatusResponse) this.ComboBoxBinaryOutputStatus.SelectedValue;
                config.analogOutputStatus = (EventAnalogOutputStatusResponse) this.ComboBoxAnalogOutputStatus.SelectedValue;

                return config;
            }
        }

    }
}
