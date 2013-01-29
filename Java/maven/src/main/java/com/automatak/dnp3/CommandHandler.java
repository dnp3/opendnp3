package com.automatak.dnp3;

public interface CommandHandler {

    CommandStatus Select(ControlRelayOutputBlock command, long index);
    CommandStatus Select(AnalogOutput command, long index);

    CommandStatus Operate(ControlRelayOutputBlock command, long index);
    CommandStatus Operate(AnalogOutput command, long index);

    CommandStatus DirectOperate(ControlRelayOutputBlock command, long index);
    CommandStatus DirectOperate(AnalogOutput command, long index);

}
