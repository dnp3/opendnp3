package com.automatak.dnp3;

public interface CommandHandler {

    CommandStatus Select(ControlRelayOutputBlock command, long index);
    CommandStatus Select(AnalogOutputInt32 command, long index);
    CommandStatus Select(AnalogOutputInt16 command, long index);
    CommandStatus Select(AnalogOutputFloat32 command, long index);
    CommandStatus Select(AnalogOutputDouble64 command, long index);

    CommandStatus Operate(ControlRelayOutputBlock command, long index);
    CommandStatus Operate(AnalogOutputInt32 command, long index);
    CommandStatus Operate(AnalogOutputInt16 command, long index);
    CommandStatus Operate(AnalogOutputFloat32 command, long index);
    CommandStatus Operate(AnalogOutputDouble64 command, long index);

    CommandStatus DirectOperate(ControlRelayOutputBlock command, long index);
    CommandStatus DirectOperate(AnalogOutputInt32 command, long index);
    CommandStatus DirectOperate(AnalogOutputInt16 command, long index);
    CommandStatus DirectOperate(AnalogOutputFloat32 command, long index);
    CommandStatus DirectOperate(AnalogOutputDouble64 command, long index);

}
