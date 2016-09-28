/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.CommandStatus;
import com.automatak.dnp3.enums.ControlCode;
import com.automatak.dnp3.enums.OperateType;

/**
 * Makes the JNI callbacks easier by breaking down the objects and passing enums as ints
 */
public class CommandHandlerAdapter {

    private final CommandHandler proxy;

    public CommandHandlerAdapter(CommandHandler proxy)
    {
        this.proxy = proxy;
    }

    // ControlRelayOutputBlock

    public int select(int function, short count, long onTimeMs, long offTimeMs, int status, int index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.select(crob, index).toType();
    }

    public int operate(int function, short count, long onTimeMs, long offTimeMs, int status, int index, int opType)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.operate(crob, index, OperateType.fromType(opType)).toType();
    }
    

    // AnalogOutputInt32

    public int select(int value, int status, int index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.fromType(status));
        return proxy.select(ao, index).toType();
    }

    public int operate(int value, int status, int index, int opType)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.fromType(status));
        return proxy.operate(ao, index, OperateType.fromType(opType)).toType();
    }

    // AnalogOutputInt16

    public int select(short value, int status, int index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.fromType(status));
        return proxy.select(ao, index).toType();
    }

    public int operate(short value, int status, int index, int opType)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.fromType(status));
        return proxy.operate(ao, index, OperateType.fromType(opType)).toType();
    }

    // AnalogOutputFloat32

    public int select(float value, int status, int index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.fromType(status));
        return proxy.select(ao, index).toType();
    }

    public int operate(float value, int status, int index, int opType)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.fromType(status));
        return proxy.operate(ao, index, OperateType.fromType(opType)).toType();
    }


    // AnalogOutputDouble64

    public int select(double value, int status, int index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.fromType(status));
        return proxy.select(ao, index).toType();
    }

    public int operate(double value, int status, int index, int opType)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.fromType(status));
        return proxy.operate(ao, index, OperateType.fromType(opType)).toType();
    }



}
