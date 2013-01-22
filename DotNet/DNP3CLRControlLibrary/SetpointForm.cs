
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
    public partial class SetpointForm : Form
    {
        ICommandProcessor cmdProcessor;

        public SetpointForm(ICommandProcessor cmdProcessor)
        {
            InitializeComponent();
            this.cmdProcessor = cmdProcessor;
        }

        private Setpoint GetSetpoint()
        {
            switch (this.comboBoxType.SelectedIndex)
            {
                case (0): 
                    return new Setpoint(Decimal.ToUInt16(this.numericUpDownValue.Value));
                case (1):
                    return new Setpoint(Decimal.ToUInt32(this.numericUpDownValue.Value));
                case (2):
                    return new Setpoint(Decimal.ToSingle(this.numericUpDownValue.Value));
                default:
                    return new Setpoint(Decimal.ToDouble(this.numericUpDownValue.Value));
            }
        }        

        private void OnResult(CommandStatus status)
        {
            Action invoke = () => OnMarshalledResult(status);            
            this.BeginInvoke(invoke);
        }

        private void OnMarshalledResult(CommandStatus status)
        {
            this.groupBoxParameters.Enabled = true;
            this.textBoxStatus.Text = status.ToString();
        }        

        private void buttonDirectOperate_Click(object sender, EventArgs e)
        {
            this.groupBoxParameters.Enabled = false;
            this.textBoxStatus.Text = "DirectOperate...";
            var future = cmdProcessor.DirectOperate(GetSetpoint(), Decimal.ToUInt32(this.numericUpDownIndex.Value));
            future.Listen(OnResult);
        }

        private void buttonSelectOperate_Click(object sender, EventArgs e)
        {
            this.groupBoxParameters.Enabled = false;
            this.textBoxStatus.Text = "SelectOperate...";
            var future = cmdProcessor.SelectAndOperate(GetSetpoint(), Decimal.ToUInt32(this.numericUpDownIndex.Value));
            future.Listen(OnResult);
        }
    }
}
