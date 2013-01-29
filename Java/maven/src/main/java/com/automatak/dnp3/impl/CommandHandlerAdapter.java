package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;

// Makes the JNI callbacks easier by breaking down the objects and passing enums as ints
class CommandHandlerAdapter {

    private final CommandHandler proxy;

    public CommandHandlerAdapter(CommandHandler proxy)
    {
        this.proxy = proxy;
    }

    public int SelectCROB(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromInt(function), count, onTimeMs, offTimeMs, CommandStatus.fromInt(status));
        return proxy.Select(crob, index).toInt();
    }

    public int OperateCROB(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromInt(function), count, onTimeMs, offTimeMs, CommandStatus.fromInt(status));
        return proxy.Operate(crob, index).toInt();
    }

    public int DirectOperateCROB(int function, short count, long onTimeMs, long offTimeMs, int status, long index)
    {
        ControlRelayOutputBlock crob = new ControlRelayOutputBlock(ControlCode.fromInt(function), count, onTimeMs, offTimeMs, CommandStatus.fromInt(status));
        return proxy.DirectOperate(crob, index).toInt();
    }

}
