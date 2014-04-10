/**
 * Copyright 2013 Automatak, LLC
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
package com.automatak.dnp3.impl;

import com.automatak.dnp3.CommandResponse;
import com.automatak.dnp3.*;


class CommandProcessorImpl implements CommandProcessor {

    private final long nativeptr;

    private static CommandResultCallback fromPromise(final Promise<CommandResponse> p)
    {
        return new CommandResultCallback() {
            @Override
            public void onCommandCallback(int result, int status) {
                CommandResult r = CommandResult.fromType(result);
                CommandStatus s = CommandStatus.fromType(status);
                p.set(new CommandResponse(r,s));
            }
        };
    }

    public CommandProcessorImpl(long nativeptr)
    {
        this.nativeptr = nativeptr;
    }

    public ListenableFuture<CommandResponse> selectAndOperate(ControlRelayOutputBlock command, long index)
    {
        final BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        selectAndOperateCROB(nativeptr, command.function.toType(), command.count, command.onTimeMs, command.offTimeMs, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> selectAndOperate(AnalogOutputInt32 command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        selectAndOperateAnalogInt32(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> selectAndOperate(AnalogOutputInt16 command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        selectAndOperateAnalogInt16(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> selectAndOperate(AnalogOutputFloat32 command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        selectAndOperateAnalogFloat32(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> selectAndOperate(AnalogOutputDouble64 command, long index){
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        selectAndOperateAnalogDouble64(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }

    public ListenableFuture<CommandResponse> directOperate(ControlRelayOutputBlock command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        directOperateCROB(nativeptr, command.function.toType(), command.count, command.onTimeMs, command.offTimeMs, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> directOperate(AnalogOutputInt32 command, long index){
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        directOperateAnalogInt32(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> directOperate(AnalogOutputInt16 command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        directOperateAnalogInt16(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> directOperate(AnalogOutputFloat32 command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        directOperateAnalogFloat32(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }
    public ListenableFuture<CommandResponse> directOperate(AnalogOutputDouble64 command, long index)
    {
        BasicListenableFuture<CommandResponse> future = new BasicListenableFuture<CommandResponse>();
        directOperateAnalogDouble64(nativeptr, command.value, command.status.toType(), index, fromPromise(future));
        return future;
    }

    private native void selectAndOperateCROB(long nativeptr, int function, short count, long onTimeMs, long offTimeMs, int status, long index, CommandResultCallback callback);
    private native void directOperateCROB(long nativeptr, int function, short count, long onTimeMs, long offTimeMs, int status, long index, CommandResultCallback callback);

    private native void selectAndOperateAnalogInt32(long nativeptr, int value, int status, long index,CommandResultCallback callback);
    private native void directOperateAnalogInt32(long nativeptr, int value, int status, long index, CommandResultCallback callback);

    private native void selectAndOperateAnalogInt16(long nativeptr, short value, int status, long index, CommandResultCallback callback);
    private native void directOperateAnalogInt16(long nativeptr, short value, int status, long index, CommandResultCallback callback);

    private native void selectAndOperateAnalogFloat32(long nativeptr, float value, int status, long index, CommandResultCallback callback);
    private native void directOperateAnalogFloat32(long nativeptr, float value, int status, long index, CommandResultCallback callback);

    private native void selectAndOperateAnalogDouble64(long nativeptr, double value, int status, long index, CommandResultCallback callback);
    private native void directOperateAnalogDouble64(long nativeptr, double value, int status, long index, CommandResultCallback callback);




}
