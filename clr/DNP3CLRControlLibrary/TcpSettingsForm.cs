
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
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using DNP3.Interface;

namespace TestSetControlLibrary
{    
    public partial class TcpSettingsForm : Form
    {       
        public delegate void AddTcp(TcpSettings settings);
        public event AddTcp OnTcpAdded;               

        public TcpSettingsForm(String banner, String defaultPortName)
        {
            InitializeComponent();
            this.Text = banner;
            this.textBoxName.Text = defaultPortName;
            this.FormClosed += new FormClosedEventHandler(TcpSettingsForm_FormClosed);
        }

        void TcpSettingsForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if(DialogResult == DialogResult.OK && OnTcpAdded != null) 
            {
                String name = this.textBoxName.Text;
                String address = this.textBoxAddress.Text;
                UInt16 port = Decimal.ToUInt16(this.numericUpDownPort.Value);
                FilterLevel level = this.filterLevelComboBox1.SelectedLevel;
                UInt64 timeout = Decimal.ToUInt64(this.numericUpDownTimeout.Value);
                OnTcpAdded(new TcpSettings(name, address, port, level, timeout));            
            }
        }

        private void buttonAdd_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        
    }
}
