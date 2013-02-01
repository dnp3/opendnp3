/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3;

public interface CommandHandler {

    CommandStatus Select(ControlRelayOutputBlock command, long index);
    CommandStatus Select(AnalogOutputInt32 command, long index);
    CommandStatus Select(AnalogOutputInt16 command, long index);
    CommandStatus Select(AnalogOutputFloat32 command, long index);
    CommandStatus Select(AnalogOutputDouble64 command, long index);

    CommandStatus Operate(ControlRelayOutputBlock command, long index);
    CommandStatus Operate(AnalogOutputInt32 command, long index);
    CommandStatus Operate(AnalogOutputInt16 command, long index);
    CommandStatus Operate(AnalogOutputFloat32 command, long index);
    CommandStatus Operate(AnalogOutputDouble64 command, long index);

    CommandStatus DirectOperate(ControlRelayOutputBlock command, long index);
    CommandStatus DirectOperate(AnalogOutputInt32 command, long index);
    CommandStatus DirectOperate(AnalogOutputInt16 command, long index);
    CommandStatus DirectOperate(AnalogOutputFloat32 command, long index);
    CommandStatus DirectOperate(AnalogOutputDouble64 command, long index);

}
