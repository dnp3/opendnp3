package com.automatak.dnp3;

public interface CommandProcessor {

    ListenableFuture<CommandStatus> selectAndOperate(ControlRelayOutputBlock command, long index);
    ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputInt32 command, long index);
    ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputInt16 command, long index);
    ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputFloat32 command, long index);
    ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputDouble64 command, long index);

    ListenableFuture<CommandStatus> directOperate(ControlRelayOutputBlock command, long index);
    ListenableFuture<CommandStatus> directOperate(AnalogOutputInt32 command, long index);
    ListenableFuture<CommandStatus> directOperate(AnalogOutputInt16 command, long index);
    ListenableFuture<CommandStatus> directOperate(AnalogOutputFloat32 command, long index);
    ListenableFuture<CommandStatus> directOperate(AnalogOutputDouble64 command, long index);

}
