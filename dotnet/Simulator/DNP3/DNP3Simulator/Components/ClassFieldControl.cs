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
    public partial class ClassFieldControl : UserControl
    {
        public ClassFieldControl()
        {
            InitializeComponent();
        }

        public bool AllowClass0
        {
            set
            {
                checkBoxClass0.Enabled = value;
                if (!value)
                {
                    checkBoxClass0.Checked = false;
                }
            }
        }        

        public ClassField ClassFieldValue
        {
            get
            {
                var field = new ClassField();
                field.Class0 = checkBoxClass0.Checked;
                field.Class1 = checkBoxClass1.Checked;
                field.Class2 = checkBoxClass2.Checked;
                field.Class3 = checkBoxClass3.Checked;
                return field;
            }
            set
            {
                checkBoxClass0.Checked = value.Class0;
                checkBoxClass1.Checked = value.Class1;
                checkBoxClass2.Checked = value.Class2;
                checkBoxClass3.Checked = value.Class3;                            
            }
        }
    }
}
