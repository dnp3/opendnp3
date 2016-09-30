/**
 * Copyright 2013-2016 Automatak, LLC
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
package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.*;

public class ExpectedValue {

    enum Type {
        BinaryType,
        DoubleBinaryType,
        AnalogType,
        CounterType,
        FrozenCounterType,
        BOStatusType,
        AOStatusType
    }

    public static final ExpectedValue.Type[] ALL_TYPES = ExpectedValue.Type.values();

    final int index;
    final Measurement measurement;
    final Type type;

    public ExpectedValue(BinaryInput measurement, int index)
    {
        this(measurement, index, Type.BinaryType);
    }

    public ExpectedValue(DoubleBitBinaryInput measurement, int index)
    {
        this(measurement, index, Type.DoubleBinaryType);
    }

    public ExpectedValue(AnalogInput measurement, int index)
    {
        this(measurement, index, Type.AnalogType);
    }

    public ExpectedValue(Counter measurement, int index)
    {
        this(measurement, index, Type.CounterType);
    }

    public ExpectedValue(FrozenCounter measurement, int index)
    {
        this(measurement, index, Type.FrozenCounterType);
    }

    public ExpectedValue(BinaryOutputStatus measurement, int index)
    {
        this(measurement, index, Type.BOStatusType);
    }

    public ExpectedValue(AnalogOutputStatus measurement, int index)
    {
        this(measurement, index, Type.AOStatusType);
    }


    ExpectedValue(Measurement measurement, int index, Type type)
    {
        this.index = index;
        this.measurement = measurement;
        this.type = type;
    }

    public boolean isEqual(ExpectedValue other)
    {
        if(this.type != other.type)
        {
            return false;
        }

        if(this.index != other.index)
        {
            return false;
        }

        switch(this.type)
        {
            case BinaryType:
                return ((BinaryInput)measurement).value == ((BinaryInput) other.measurement).value;
            case DoubleBinaryType:
                return ((DoubleBitBinaryInput)measurement).value == ((DoubleBitBinaryInput) other.measurement).value;
            case AnalogType:
                return ((AnalogInput)measurement).value == ((AnalogInput) other.measurement).value;
            case CounterType:
                return ((Counter)measurement).value == ((Counter) other.measurement).value;
            case FrozenCounterType:
                return ((FrozenCounter)measurement).value == ((FrozenCounter) other.measurement).value;
            case BOStatusType:
                return ((BinaryOutputStatus)measurement).value == ((BinaryOutputStatus) other.measurement).value;
            case AOStatusType:
                return ((AnalogOutputStatus)measurement).value == ((AnalogOutputStatus) other.measurement).value;
            default:
                throw new RuntimeException("unknown type: " + this.type);
        }

    }

    @Override
    public String toString()
    {
        return String.format("%d - %s", index, this.measurement);
    }

}

