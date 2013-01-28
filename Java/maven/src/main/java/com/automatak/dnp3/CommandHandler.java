package com.automatak.dnp3;

public interface CommandHandler {

    public CommandStatus Select(ControlRelayOutputBlock command);
    public CommandStatus Select(AnalogOutput command);

    public CommandStatus Operate(ControlRelayOutputBlock command);
    public CommandStatus Operate(AnalogOutput command);

    public CommandStatus DirectOperate(ControlRelayOutputBlock command);
    public CommandStatus DirectOperate(AnalogOutput command);

}
