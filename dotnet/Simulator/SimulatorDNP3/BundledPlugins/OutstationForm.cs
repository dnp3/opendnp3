using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.Simulator.DNP3
{    
    partial class OutstationForm : Form
    {
        MeasurementCollection activeCollection = null;

        readonly IOutstation outstation;
        readonly IMeasurementCache cache;

        public OutstationForm(IOutstation outstation, IMeasurementCache cache, String alias)
        {
            InitializeComponent();

            this.outstation = outstation;
            this.cache = cache;

            this.Text = String.Format("DNP3 Outstation ({0})", alias);
            this.comboBoxTypes.DataSource = System.Enum.GetValues(typeof(MeasType));

            this.CheckState();
        }       
        
        void CheckState()
        {
            if (((MeasType)comboBoxTypes.SelectedValue) != MeasType.OctetString && this.measurementView.SelectedIndices.Any())
            {
                this.buttonEdit.Enabled = true;
            }
            else
            {
                this.buttonEdit.Enabled = false;
            }
        }
     
        void GUIMasterForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
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
            this.CheckState(); 
        }

        private void measurementView_OnRowSelectionChanged(IEnumerable<UInt16> rows)
        {
            this.CheckState();
        }                                                       
    }
}
