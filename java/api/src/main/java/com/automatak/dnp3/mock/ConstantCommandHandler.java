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
package com.automatak.dnp3.mock;


import com.automatak.dnp3.*;

public class ConstantCommandHandler implements CommandHandler {

    private final CommandStatus status;

    public ConstantCommandHandler(CommandStatus status)
    {
        this.status = status;
    }
    public CommandStatus select(ControlRelayOutputBlock command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputInt32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputInt16 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputFloat32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputDouble64 command, long index, byte sequence)
    {
        return status;
    }

    public CommandStatus operate(ControlRelayOutputBlock command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputInt32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputInt16 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputFloat32 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputDouble64 command, long index, byte sequence)
    {
        return status;
    }
    public CommandStatus directOperate(ControlRelayOutputBlock command, long index)
    {
        return status;
    }
    public CommandStatus directOperate(AnalogOutputInt32 command, long index)
    {
        return status;
    }
    public CommandStatus directOperate(AnalogOutputInt16 command, long index)
    {
        return status;
    }
    public CommandStatus directOperate(AnalogOutputFloat32 command, long index)
    {
        return status;
    }
    public CommandStatus directOperate(AnalogOutputDouble64 command, long index)
    {
        return status;
    }
}
