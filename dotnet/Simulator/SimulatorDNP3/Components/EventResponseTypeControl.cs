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
    public partial class EventResponseTypeControl : ResponseTypeControl
    {
        public EventResponseTypeControl()
        {
            InitializeComponent();

            this.ComboBoxBinary.DataSource = Enum.GetValues(typeof(EventBinaryVariation));
            this.ComboBoxDoubleBinary.DataSource = Enum.GetValues(typeof(EventDoubleBinaryVariation));
            this.ComboBoxCounter.DataSource = Enum.GetValues(typeof(EventCounterVariation));
            this.ComboBoxFrozenCounter.DataSource = Enum.GetValues(typeof(EventFrozenCounterVariation));
            this.ComboBoxAnalog.DataSource = Enum.GetValues(typeof(EventAnalogVariation));
            this.ComboBoxBinaryOutputStatus.DataSource = Enum.GetValues(typeof(EventBinaryOutputStatusVariation));
            this.ComboBoxAnalogOutputStatus.DataSource = Enum.GetValues(typeof(EventAnalogOutputStatusVariation));
        }        

    }
}
