using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{    
    partial class MasterForm : Form
    {
        MeasurementCollection activeCollection = null;

        readonly IMaster master;
        readonly IMeasurementCache cache;      

        public MasterForm(IMaster master, IMeasurementCache cache, String alias)
        {
            InitializeComponent();

            this.master = master;
            this.cache = cache;

            this.Text = String.Format("DNP3 Master ({0})", alias);

            this.masterScanControl1.Master = master;
        }                       
     
        void GUIMasterForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }       

        void GUIMasterForm_Load(object sender, EventArgs e)
        {
            this.comboBoxTypes.DataSource = null;
            this.comboBoxTypes.DataSource = System.Enum.GetValues(typeof(MeasType));            
        }

        void comboBoxTypes_SelectedIndexChanged(object sender, EventArgs e)
        {
            var index = this.comboBoxTypes.SelectedIndex;
            if(Enum.IsDefined(typeof(MeasType), index))
            {
                MeasType type = (MeasType) Enum.ToObject(typeof(MeasType), index);             
                var collection = cache.GetCollection(type);
                if (collection != null)
                {
                    if (activeCollection != null)
                    {
                        activeCollection.RemoveObserver(this.measurementView);
                    }

                    activeCollection = collection;

                    collection.AddObserver(this.measurementView);
                }                
            }                      
        }

        private void buttonDirectOperate_Click(object sender, EventArgs e)
        {
            var crob = crobControl1.ControlValue;
            var future = this.master.GetCommandProcessor().DirectOperate(crob, 0);
        }                                
        
    }
}
