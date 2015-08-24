using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.Commons;

namespace Automatak.Simulator.DNP3.DefaultOutstationPlugin
{    
    partial class OutstationForm : Form
    {
        MeasurementCollection activeCollection = null;

        readonly IOutstation outstation;
        readonly EventedOutstationApplication application;
        readonly MeasurementCache cache;
        readonly ProxyCommandHandler proxy;
        readonly IMeasurementLoader loader;        

        readonly ChangeSet events = new ChangeSet();

        public OutstationForm(IOutstation outstation, EventedOutstationApplication application, MeasurementCache cache, ProxyCommandHandler proxy, String alias)
        {
            InitializeComponent();

            this.outstation = outstation;
            this.application = application;
            this.cache = cache;
            this.proxy = proxy;

            this.loader = new ProxyLoader(outstation, cache);

            this.Text = String.Format("DNP3 Outstation ({0})", alias);
            this.comboBoxTypes.DataSource = System.Enum.GetValues(typeof(MeasType));

            this.commandHandlerControl1.Configure(proxy, loader);

            this.comboBoxColdRestartMode.DataSource = System.Enum.GetValues(typeof(RestartMode));

            this.application.ColdRestart += application_ColdRestart;
            this.application.TimeWrite += application_TimeWrite;

            this.CheckState();
        }

        void application_TimeWrite(ulong millisecSinceEpoch)
        {
            this.application.NeedTime = false;
            this.Invoke(new Action(() => this.checkBoxNeedTime.Checked = false));
        }

        void application_ColdRestart()
        {
            // simulate a restart with the restart IIN bit
            this.outstation.SetRestartIIN();
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
                    LoadBinaries(indices, true);
                    break;
                case (MeasType.BinaryOutputStatus):
                    LoadBinaries(indices, false);
                    break;
                case (MeasType.Counter):
                    LoadCounters(indices, true);
                    break;
                case (MeasType.FrozenCounter):
                    LoadCounters(indices, false);
                    break;
                case (MeasType.Analog):
                    LoadAnalogs(indices, true);
                    break;
                case (MeasType.AnalogOutputStatus):
                    LoadAnalogs(indices, false);
                    break;
                case(MeasType.DoubleBitBinary):
                    LoadDoubleBinaries(indices);
                    break;
                
            }
        }

        void LoadBinaries(IEnumerable<ushort> indices, bool isBinary)
        {
            using (var dialog = new BinaryValueDialog(isBinary, indices))
            {                
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.AddChanges(dialog.SelectedChanges);
                }
            }
        }

        void LoadDoubleBinaries(IEnumerable<ushort> indices)
        {
            using (var dialog = new DoubleBinaryValueDialog(indices))
            {                
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.AddChanges(dialog.SelectedChanges);
                }
            }
        }

        void LoadAnalogs(IEnumerable<ushort> indices, bool isAnalog)
        {
            using (var dialog = new AnalogValueDialog(isAnalog, indices))
            {                
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.AddChanges(dialog.SelectedChanges);
                }
            }
        }

        void LoadCounters(IEnumerable<ushort> indices, bool isCounter)
        {
            using (var dialog = new CounterValueDialog(isCounter, indices))
            {                
                if (dialog.ShowDialog() == DialogResult.OK)
                {
                    this.AddChanges(dialog.SelectedChanges);
                }
            }
        }

        void AddChanges(IChangeSet changes)
        {
            ListviewDatabaseAdapter.Process(changes, listBoxEvents);

            // merge these changes onto the main changeset
            changes.Apply(events);                        
            
            this.CheckState();
        }

        private void measurementView_OnRowSelectionChanged(IEnumerable<UInt16> selection)
        {
            this.CheckState();
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {           
           loader.Load(events);
           events.Clear();           
           
           this.listBoxEvents.Items.Clear();           
           this.CheckState();
        }

        private void buttonClear_Click(object sender, EventArgs e)
        {
            this.listBoxEvents.Items.Clear();
            this.events.Clear();
            this.CheckState();
        }       

        private void checkBoxNeedTime_CheckedChanged(object sender, EventArgs e)
        {
            this.application.SupportsWriteTime = checkBoxNeedTime.Checked;
            this.application.NeedTime = checkBoxNeedTime.Checked;                        
        }

        private void checkBoxLocalMode_CheckedChanged(object sender, EventArgs e)
        {
            this.application.LocalMode = checkBoxLocalMode.Checked;
        }

        private void comboBoxColdRestartMode_SelectedValueChanged(object sender, EventArgs e)
        {
            this.application.ColdRestartMode = (RestartMode) comboBoxColdRestartMode.SelectedValue;
        }

        private void numericUpDownColdRestartTime_ValueChanged(object sender, EventArgs e)
        {
            this.application.ColdRestartTime = Decimal.ToUInt16(numericUpDownColdRestartTime.Value);
        }       
    }
}
