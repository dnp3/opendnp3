
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

using DNP3.Adapter;
using DNP3.Interface;
using TestSetControlLibrary;

namespace DotNetTestSet
{
    public partial class TestSetForm : Form
    {
        private StackManager sm;
        private LogControlAdapter lca;

        public TestSetForm()
        {           
            InitializeComponent();

            this.sm = new StackManager((UInt32)System.Environment.ProcessorCount);
            this.lca = new LogControlAdapter(this.logControl);
            sm.AddLogHandler(lca);            
        }        

        private void stackBrowser1_OnTcpClientAdded(TcpSettings s)
        {
            sm.AddTCPClient(s.name, s.level, s.timeout, s.address, s.port);                            
        }

        private void stackBrowser1_OnTcpServerAdded(TcpSettings s)
        {
            sm.AddTCPServer(s.name, s.level, s.timeout, s.address, s.port);
        }

        private void stackBrowser1_OnSerialPortAdded(SerialSettings settings)
        {
            sm.AddSerial(settings.port, FilterLevel.LEV_WARNING, 5000, settings);
        }

        private void stackBrowser1_OnAddMaster(string name, string port, FilterLevel level, MasterStackConfig config)
        {            
            var observer = new EventedDataObserver(this);
            var control = this.stackDisplayControl.AddMaster(name, observer.MeasurementSource);             
            control.CommandProcessor = sm.AddMaster(port, name, level, observer, config);                            
        }

        private void stackBrowser1_OnRemovePort(string name)
        {
            sm.RemovePort(name);
        }

        private void stackBrowser1_OnRemoveStack(string name)
        {
            sm.RemoveStack(name);
        }     
        

        
    }

    
    
}