using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{    
    public partial class GUIMasterForm : Form
    {
        private SOEHandler handler = new SOEHandler();

        private MeasurementCollection binaries = new MeasurementCollection();
        private MeasurementCollection doubleBinaries = new MeasurementCollection();
        private MeasurementCollection counters = new MeasurementCollection();
        private MeasurementCollection frozenCounters = new MeasurementCollection();
        private MeasurementCollection analogs = new MeasurementCollection();
        private MeasurementCollection binaryOutputStatii = new MeasurementCollection();
        private MeasurementCollection analogOutputStatii = new MeasurementCollection();
        private MeasurementCollection octetStrings = new MeasurementCollection();

        private MeasurementCollection activeCollection;
        private IMaster master;
        private IMasterScan scan;

        public ISOEHandler SequenceOfEvents
        {
            get { return handler; }            
        }

        public GUIMasterForm()
        {
            InitializeComponent();            
            handler.NewMeasurements += handler_NewMeasurements;
        }

        public void SetMaster(IMaster master)
        {
            this.master = master;
            this.scan = master.AddClassScan(~0, TimeSpan.FromMinutes(1));
        }

        // this event comes from an non-UI thread and needs to be synchronized
        void handler_NewMeasurements(IEnumerable<Measurement> measurements)
        {
            foreach (var meas in measurements)
            {
                var collection = GetCollectionMaybeNull(meas.Type);
                if (collection != null)
                {
                    collection.Update(meas);
                }                
            }
        }             
     

        private void GUIMasterForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }       

        private void GUIMasterForm_Load(object sender, EventArgs e)
        {
            this.comboBoxTypes.DataSource = null;
            this.comboBoxTypes.DataSource = System.Enum.GetValues(typeof(MeasType));            
        }

        private void comboBoxTypes_SelectedIndexChanged(object sender, EventArgs e)
        {
            var index = this.comboBoxTypes.SelectedIndex;
            if(Enum.IsDefined(typeof(MeasType), index))
            {
                MeasType type = (MeasType) Enum.ToObject(typeof(MeasType), index);
                var collection = GetCollectionMaybeNull(type);
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

        private MeasurementCollection GetCollectionMaybeNull(MeasType type)
        {
            switch(type)
            {
                case(MeasType.Binary):
                    return binaries;                        
                case (MeasType.DoubleBitBinary):
                    return doubleBinaries;
                case(MeasType.Counter):
                    return counters;
                case(MeasType.FrozenCounter):
                    return frozenCounters;
                case(MeasType.Analog):
                    return analogs;
                case(MeasType.BinaryOutputStatus):
                    return binaryOutputStatii;
                case(MeasType.AnalogOutputStatus):
                    return analogOutputStatii;
                case(MeasType.OctetString):
                    return octetStrings;
                default:
                    return null;
            }
        }

        private void OnCommandResult(CommandResponse rsp)
        {
            //this.button1.Enabled = true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
           // button1.Enabled = false;
            var cp = master.GetCommandProcessor();
            var future = cp.SelectAndOperate(new ControlRelayOutputBlock(ControlCode.LATCH_ON, 1, 100, 100), 0);
            future.Listen(rsp => this.BeginInvoke(new Action(() => OnCommandResult(rsp))));
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            this.scan.Demand();
        }
        
    }
}
