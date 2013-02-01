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

    public int Select(int function, short count, long onTimeMs, long offTimeMs, int status, long index, byte aSeq)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromInt(function), count, onTimeMs, offTimeMs, CommandStatus.fromInt(status));
        return proxy.Select(crob, index).toInt();
    }

    public int Operate(int function, short count, long onTimeMs, long offTimeMs, int status, long index, byte aSeq)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromInt(function), count, onTimeMs, offTimeMs, CommandStatus.fromInt(status));
        return proxy.Operate(crob, index).toInt();
    }

    public int DirectOperate(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromInt(function), count, onTimeMs, offTimeMs, CommandStatus.fromInt(status));
        return proxy.DirectOperate(crob, index).toInt();
    }

    // AnalogOutputInt32

    public int Select(int value, long index, byte aSeq)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.Select(ao, index).toInt();
    }

    public int Operate(int value, long index, byte aSeq)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.Operate(ao, index).toInt();
    }

    public int DirectOperate(int value, long index)
    {
        AnalogOutputInt32 ao = new AnalogOutputInt32(value, CommandStatus.SUCCESS);
        return proxy.DirectOperate(ao, index).toInt();
    }

    // AnalogOutputInt16

    public int Select(short value, long index, byte aSeq)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.Select(ao, index).toInt();
    }

    public int Operate(short value, long index, byte aSeq)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.Operate(ao, index).toInt();
    }

    public int DirectOperate(short value, long index)
    {
        AnalogOutputInt16 ao = new AnalogOutputInt16(value, CommandStatus.SUCCESS);
        return proxy.DirectOperate(ao, index).toInt();
    }

    // AnalogOutputFloat32

    public int Select(float value, long index, byte aSeq)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.Select(ao, index).toInt();
    }

    public int Operate(float value, long index, byte aSeq)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.Operate(ao, index).toInt();
    }

    public int DirectOperate(float value, long index)
    {
        AnalogOutputFloat32 ao = new AnalogOutputFloat32(value, CommandStatus.SUCCESS);
        return proxy.DirectOperate(ao, index).toInt();
    }

    // AnalogOutputDouble64

    public int Select(double value, long index, byte aSeq)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.Select(ao, index).toInt();
    }

    public int Operate(double value, long index, byte aSeq)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.Operate(ao, index).toInt();
    }

    public int DirectOperate(double value, long index)
    {
        AnalogOutputDouble64 ao = new AnalogOutputDouble64(value, CommandStatus.SUCCESS);
        return proxy.DirectOperate(ao, index).toInt();
    }

}
