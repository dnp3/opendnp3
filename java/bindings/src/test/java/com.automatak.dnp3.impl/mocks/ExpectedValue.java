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

