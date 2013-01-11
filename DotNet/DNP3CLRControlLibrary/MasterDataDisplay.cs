using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace TestSetControlLibrary
{
    public partial class MasterDataDisplay : UserControl, IMasterControl
    {
        private ICommandAcceptor cmdAcceptor = null;
        private IMeasurementSource source;

        public MasterDataDisplay(IMeasurementSource source)
        {
            InitializeComponent();        
            this.source = source;
            source.AllBinaryUpdate += new OnUpdateBinary(this.UpdateBinary);
            source.AllAnalogUpdate += new OnUpdateAnalog(this.UpdateAnalog);
            source.AllCounterUpdate += new OnUpdateCounter(this.UpdateCounter);
            source.AllControlStatusUpdate += new OnUpdateControlStatus(this.UpdateBOStatus);
            source.AllSetpointStatusUpdate += new OnUpdateSetpointStatus(this.UpdateAOStatus);
        }

        public ICommandAcceptor CommandAcceptor
        {
            set
            {
                this.cmdAcceptor = value;
            }

        }

        private ListViewItem RenderBinary(Binary meas, UInt32 index, String id)
        {
            return new ListViewItem(new String[] { index.ToString(), meas.value.ToString() });
        }

        private ListViewItem RenderAnalog(Analog meas, UInt32 index, String id)
        {
            return new ListViewItem(new String[] { index.ToString(), meas.value.ToString() });
        }

        private ListViewItem RenderCounter(Counter meas, UInt32 index, String id)
        {
            return new ListViewItem(new String[] { index.ToString(), meas.value.ToString() });
        }

        private ListViewItem RenderBOStatus(ControlStatus meas, UInt32 index, String id)
        {
            return new ListViewItem(new String[] { index.ToString(), meas.value.ToString() });
        }

        private ListViewItem RenderAOStatus(SetpointStatus meas, UInt32 index, String id)
        {
            return new ListViewItem(new String[] { index.ToString(), meas.value.ToString() });
        }        

        private void UpdateBinary(Binary meas, UInt32 value, String id)
        {
            Update(meas, value, id, flickerFreeListViewBinary, RenderBinary);
        }

        private void UpdateAnalog(Analog meas, UInt32 value, String id)
        {
            Update(meas, value, id, flickerFreeListViewAnalog, RenderAnalog);
        }

        private void UpdateCounter(Counter meas, UInt32 value, String id)
        {
            Update(meas, value, id, flickerFreeListViewCounter, RenderCounter);
        }

        private void UpdateBOStatus(ControlStatus meas, UInt32 value, String id)
        {
            Update(meas, value, id, flickerFreeListViewBOStatus, RenderBOStatus);
        }

        private void UpdateAOStatus(SetpointStatus meas, UInt32 value, String id)
        {
            Update(meas, value, id, flickerFreeListViewAOStatus, RenderAOStatus);
        }

        private static void Update<T>(T meas, UInt32 index, String id, FlickerFreeListView view, Func<T, UInt32, String, ListViewItem> render)
        {                    
            int idx = (int) index;

            if (idx >= view.Items.Count) //add items to the view
            {
                for (int i = view.Items.Count; i <= index; ++i)
                {
                    view.Items.Add(i.ToString());
                }
            }
            
            view.Items[idx] = render(meas, index, id);  
        }

        private void analogOutputToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var form = new SetpointForm(this.cmdAcceptor);
            form.Show();
        }
    }
}
