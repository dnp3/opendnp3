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

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{
    public partial class QualitySelector : UserControl
    {
        public QualitySelector()
        {
            InitializeComponent();
        }

        public IQualityBitInfo QualityInfo
        {
            set {
                this.checkBox0.Text = value.GetLongLabel(0);
                this.checkBox1.Text = value.GetLongLabel(1);
                this.checkBox2.Text = value.GetLongLabel(2);
                this.checkBox3.Text = value.GetLongLabel(3);
                this.checkBox4.Text = value.GetLongLabel(4);
                this.checkBox5.Text = value.GetLongLabel(5);
                this.checkBox6.Text = value.GetLongLabel(6);
                this.checkBox7.Text = value.GetLongLabel(7);
            }
        }

        public byte Quality
        {
            get
            {
                byte value = 0;
                if (this.checkBox0.Checked) value |= 1 << 0;
                if (this.checkBox1.Checked) value |= 1 << 1;
                if (this.checkBox2.Checked) value |= 1 << 2;
                if (this.checkBox3.Checked) value |= 1 << 3;
                if (this.checkBox4.Checked) value |= 1 << 4;
                if (this.checkBox5.Checked) value |= 1 << 5;
                if (this.checkBox6.Checked) value |= 1 << 6;
                if (this.checkBox7.Checked) value |= 1 << 7;
                return value;
            }
        }
    }
}
