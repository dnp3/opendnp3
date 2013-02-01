package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;


public class CommandProcessorImpl implements CommandProcessor {

    private final long nativeptr;

    private interface StatusCallback
    {
        void onStatusCallback(int status);
    }

    private static StatusCallback fromPromise(final Promise<CommandStatus> p)
    {
        return new StatusCallback() {
            @Override
            public void onStatusCallback(int status) {
                p.set(CommandStatus.fromInt(status));
            }
        };
    }

    public CommandProcessorImpl(long nativeptr)
    {
        this.nativeptr = nativeptr;
    }

    public ListenableFuture<CommandStatus> selectAndOperate(ControlRelayOutputBlock command, long index)
    {
        final BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        selectAndOperateCROB(nativeptr, command.function.toInt(), command.count, command.onTimeMs, command.offTimeMs, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputInt32 command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        selectAndOperateAnalogInt32(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputInt16 command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        selectAndOperateAnalogInt16(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputFloat32 command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        selectAndOperateAnalogFloat32(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> selectAndOperate(AnalogOutputDouble64 command, long index){
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        selectAndOperateAnalogDouble64(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }

    public ListenableFuture<CommandStatus> directOperate(ControlRelayOutputBlock command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        directOperateCROB(nativeptr, command.function.toInt(), command.count, command.onTimeMs, command.offTimeMs, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> directOperate(AnalogOutputInt32 command, long index){
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        directOperateAnalogInt32(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> directOperate(AnalogOutputInt16 command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        directOperateAnalogInt16(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> directOperate(AnalogOutputFloat32 command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        directOperateAnalogFloat32(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandStatus> directOperate(AnalogOutputDouble64 command, long index)
    {
        BasicListenableFuture<CommandStatus> future = new BasicListenableFuture<CommandStatus>();
        directOperateAnalogDouble64(nativeptr, command.value, command.status.toInt(), index, fromPromise(future));
        return future;
    }

    private native void selectAndOperateCROB(long nativeptr, int function, short count, long onTimeMs, long offTimeMs, int status, long index, StatusCallback callback);
    private native void directOperateCROB(long nativeptr, int function, short count, long onTimeMs, long offTimeMs, int status, long index, StatusCallback callback);

    private native void selectAndOperateAnalogInt32(long nativeptr, int value, int status, long index, StatusCallback callback);
    private native void directOperateAnalogInt32(long nativeptr, int value, int status, long index, StatusCallback callback);

    private native void selectAndOperateAnalogInt16(long nativeptr, short value, int status, long index, StatusCallback callback);
    private native void directOperateAnalogInt16(long nativeptr, short value, int status, long index, StatusCallback callback);

    private native void selectAndOperateAnalogFloat32(long nativeptr, float value, int status, long index, StatusCallback callback);
    private native void directOperateAnalogFloat32(long nativeptr, float value, int status, long index, StatusCallback callback);

    private native void selectAndOperateAnalogDouble64(long nativeptr, double value, int status, long index, StatusCallback callback);
    private native void directOperateAnalogDouble64(long nativeptr, double value, int status, long index, StatusCallback callback);




}
