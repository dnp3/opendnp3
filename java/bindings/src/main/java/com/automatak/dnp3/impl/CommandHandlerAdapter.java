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

    public int Supports(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.supports(crob, index).toType();
    }

    public int Perform(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromType(function), count, onTimeMs, offTimeMs, CommandStatus.fromType(status));
        return proxy.perform(crob, index).toType();
    }

    // AnalogOutputInt32

    public int Supports(int value, long index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.supports(ao, index).toType();
    }

    public int Perform(int value, long index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.perform(ao, index).toType();
    }

    // AnalogOutputInt16

    public int Supports(short value, long index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.supports(ao, index).toType();
    }

    public int Perform(short value, long index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.perform(ao, index).toType();
    }

    // AnalogOutputFloat32

    public int Supports(float value, long index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.supports(ao, index).toType();
    }

    public int Perform(float value, long index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.perform(ao, index).toType();
    }

    // AnalogOutputDouble64

    public int Supports(double value, long index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.supports(ao, index).toType();
    }

    public int Perform(double value, long index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.perform(ao, index).toType();
    }

}
