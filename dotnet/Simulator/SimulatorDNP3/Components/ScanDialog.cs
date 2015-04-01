using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3.Components
{
    public partial class ScanDialog : Form
    {
        readonly IMaster master;
        readonly IList<IMasterScan> scans = new List<IMasterScan>();

        public ScanDialog(IMaster master)
        {
            InitializeComponent();

            this.master = master;
        }

        private void buttonOnce_Click(object sender, EventArgs e)
        {
            master.Scan(GetClassHeaders());
        }

        private IEnumerable<Header> GetClassHeaders()
        { 
            IList<Header> headers = new List<Header>();
            if (classFieldControlScan.ClassFieldValue.Class0)
            {
                headers.Add(Header.AllObjects(60, 1));
            }
            if (classFieldControlScan.ClassFieldValue.Class1)
            {
                headers.Add(Header.AllObjects(60, 2));
            }
            if (classFieldControlScan.ClassFieldValue.Class2)
            {
                headers.Add(Header.AllObjects(60, 3));
            }
            if (classFieldControlScan.ClassFieldValue.Class3)
            {
                headers.Add(Header.AllObjects(60, 4));
            }
            return headers;
        }

        private void buttonBind_Click(object sender, EventArgs e)
        {
            var period = TimeSpan.FromMilliseconds(Convert.ToDouble(this.numericUpDownPeriod.Value));
            this.scans.Add(master.AddScan(GetClassHeaders(), period));
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        public IEnumerable<IMasterScan> ConfiguredScans
        {
            get
            {
                return scans;
            }
        }


    }
}
