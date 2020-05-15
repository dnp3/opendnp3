/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.dnp3.mock;


import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.CommandStatus;
import com.automatak.dnp3.enums.OperateType;

public class ConstantCommandHandler implements CommandHandler {

    private final CommandStatus status;

    public ConstantCommandHandler(CommandStatus status)
    {
        this.status = status;
    }

    @Override
    public void begin() {}

    @Override
    public void end() {}
    
    public CommandStatus select(ControlRelayOutputBlock command, int index)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputInt32 command, int index)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputInt16 command, int index)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputFloat32 command, int index)
    {
        return status;
    }
    public CommandStatus select(AnalogOutputDouble64 command, int index)
    {
        return status;
    }

    public CommandStatus operate(ControlRelayOutputBlock command, int index, Database database, OperateType opType)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputInt32 command, int index, Database database, OperateType opType)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputInt16 command, int index, Database database, OperateType opType)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputFloat32 command, int index, Database database, OperateType opType)
    {
        return status;
    }
    public CommandStatus operate(AnalogOutputDouble64 command, int index, Database database, OperateType opType)
    {
        return status;
    }
    
}
