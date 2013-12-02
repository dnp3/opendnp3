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

// Makes the JNI callbacks easier by breaking down the objects and passing enums as ints
class CommandHandlerAdapter {

    private final CommandHandler proxy;

    public CommandHandlerAdapter(CommandHandler proxy)
    {
        this.proxy = proxy;
    }

    // ControlRelayOutputBlock

    public int Select(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.select(crob, index).toType();
    }

    public int Operate(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.operate(crob, index).toType();
    }

    public int DirectOperate(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.directOperate(crob, index).toType();
    }

    // AnalogOutputInt32

    public int Select(int value, long index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.select(ao, index).toType();
    }

    public int Operate(int value, long index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.operate(ao, index).toType();
    }

    public int DirectOperate(int value, long index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.directOperate(ao, index).toType();
    }

    // AnalogOutputInt16

    public int Select(short value, long index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.select(ao, index).toType();
    }

    public int Operate(short value, long index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.operate(ao, index).toType();
    }

    public int DirectOperate(short value, long index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.directOperate(ao, index).toType();
    }

    // AnalogOutputFloat32

    public int Select(float value, long index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.select(ao, index).toType();
    }

    public int Operate(float value, long index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.operate(ao, index).toType();
    }

    public int DirectOperate(float value, long index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.directOperate(ao, index).toType();
    }

    // AnalogOutputDouble64

    public int Select(double value, long index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.select(ao, index).toType();
    }

    public int Operate(double value, long index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.operate(ao, index).toType();
    }

    public int DirectOperate(double value, long index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.directOperate(ao, index).toType();
    }

}
