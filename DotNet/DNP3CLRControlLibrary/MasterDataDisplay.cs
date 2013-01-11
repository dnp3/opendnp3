
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
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
