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
        readonly MeasurementCache cache;

        readonly IList<Action<IDatabase, DateTime>> events = new List<Action<IDatabase, DateTime>>();

        public OutstationForm(IOutstation outstation, MeasurementCache cache, String alias)
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

            if (events.Count > 0)
            {
                this.buttonApply.Enabled = true;
                this.buttonClear.Enabled = true;
            }
            else
            {
                this.buttonApply.Enabled = false;
                this.buttonClear.Enabled = false;
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

        private void buttonEdit_Click(object sender, EventArgs e)
        {
            var indices = this.measurementView.SelectedIndices;

            switch ((MeasType) comboBoxTypes.SelectedValue)
            { 
                case(MeasType.Binary):
                    LoadBinaries(indices);
                    break;
            }
        }

        void LoadBinaries(IEnumerable<ushort> indices)
        {
            using (var dialog = new BinaryValueDialog())
            {
                dialog.ShowDialog();
                if (dialog.DialogResult == DialogResult.OK)
                {
                    var meas = new Binary(dialog.SelectedValue, dialog.SelectedQuality);                    
                    var updates = indices.Select(i => GetAction(meas, i));
                    foreach(var item in updates)
                    {
                        this.events.Add(item);
                    }
                    this.CheckState();
                }
            }
        }

        Action<IDatabase, DateTime> GetAction(Binary meas, ushort index)
        {
            return (IDatabase db, DateTime timestamp) =>
            {
                meas.Timestamp = timestamp;
                db.Update(meas, index);
            };
        }

        private void measurementView_OnRowSelectionChanged(IEnumerable<UInt16> selection)
        {
            this.CheckState();
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {          
           IDatabase db1 = this.cache;
           IDatabase db2 = outstation.GetDatabase();
           var time = DateTime.Now;
           
           db1.Start();
           db2.Start();
            
           foreach (var item in events)
           {
               item.Invoke(db1, time);
               item.Invoke(db2, time);
           }

           db2.End();
           db1.End();

           this.events.Clear();

           this.CheckState();
        }                                                             
    }
}
