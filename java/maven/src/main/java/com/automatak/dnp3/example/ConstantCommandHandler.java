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
package com.automatak.dnp3.example;


import com.automatak.dnp3.*;

public class ConstantCommandHandler implements CommandHandler {

    private final CommandStatus status;

    public ConstantCommandHandler(CommandStatus status)
    {
        this.status = status;
    }
    public CommandStatus Select(ControlRelayOutputBlock command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputInt32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputInt16 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputFloat32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputDouble64 command, long index, byte sequence)
    {
        return status;
    }

    public CommandStatus Operate(ControlRelayOutputBlock command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputInt32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputInt16 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputFloat32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputDouble64 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus DirectOperate(ControlRelayOutputBlock command, long index)
    {
        return status;
    }
    public CommandStatus DirectOperate(AnalogOutputInt32 command, long index)
    {
        return status;
    }
    public CommandStatus DirectOperate(AnalogOutputInt16 command, long index)
    {
        return status;
    }
    public CommandStatus DirectOperate(AnalogOutputFloat32 command, long index)
    {
        return status;
    }
    public CommandStatus DirectOperate(AnalogOutputDouble64 command, long index)
    {
        return status;
    }
}
