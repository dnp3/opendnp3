package com.automatak.dnp3.example;


import com.automatak.dnp3.*;

public class ConstantCommandHandler implements CommandHandler {

    private final CommandStatus status;

    public ConstantCommandHandler(CommandStatus status)
    {
        this.status = status;
    }
    public CommandStatus Select(ControlRelayOutputBlock command, long index)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputInt32 command, long index)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputInt16 command, long index)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputFloat32 command, long index)
    {
        return status;
    }
    public CommandStatus Select(AnalogOutputDouble64 command, long index)
    {
        return status;
    }

    public CommandStatus Operate(ControlRelayOutputBlock command, long index)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputInt32 command, long index)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputInt16 command, long index)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputFloat32 command, long index)
    {
        return status;
    }
    public CommandStatus Operate(AnalogOutputDouble64 command, long index)
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
