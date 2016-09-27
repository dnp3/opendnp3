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

import com.automatak.dnp3.*;

import java.time.Duration;
import java.util.Arrays;
import java.util.concurrent.CompletableFuture;

class MasterImpl implements Master {

    private long nativePointer;

    @Override
    public synchronized void enable()
    {
        if(nativePointer != 0) {
            enable_native(nativePointer);
        }
    }

    @Override
    public synchronized void disable()
    {
        if(nativePointer != 0) {
            disable_native(nativePointer);
        }
    }

    @Override
    public synchronized void shutdown()
    {
        if(nativePointer != 0) {
            shutdown_native(nativePointer);
            nativePointer = 0;
        }
    }

    @Override
    public void scan(Iterable<Header> headers)
    {
        this.scan_native(this.nativePointer, headers);
    }

    @Override
    public void addPeriodicScan(Duration period, Header header)
    {
        this.add_periodic_scan_native(this.nativePointer, period, Arrays.asList(header));
    }

    @Override
    public void scan(Header header)
    {
        this.scan_native(this.nativePointer, Arrays.asList(header));
    }

    @Override
    public void addPeriodicScan(Duration period, Iterable<Header> headers)
    {
        this.add_periodic_scan_native(this.nativePointer, period, headers);
    }

    public MasterImpl(long nativePointer)
    {
        this.nativePointer = nativePointer;
    }

    @Override
    public CompletableFuture<CommandTaskResult> selectAndOperate(CommandHeaders headers)
    {
        CompletableFuture<CommandTaskResult> result = new CompletableFuture<>();
        this.select_and_operate_native(nativePointer, headers, result);
        return result;
    }

    @Override
    public CompletableFuture<CommandTaskResult> directOperate(CommandHeaders headers)
    {
        CompletableFuture<CommandTaskResult> result = new CompletableFuture<>();
        this.direct_operate_native(nativePointer, headers, result);
        return result;
    }

    @Override
    public CompletableFuture<CommandTaskResult> selectAndOperateCROB(ControlRelayOutputBlock command, int index)
    {
        return selectAndOperate(CommandHeader.fromSingleCROB(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> selectAndOperateAOInt32(AnalogOutputInt32 command, int index)
    {
        return selectAndOperate(CommandHeader.fromSingleAO32(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> selectAndOperateAOInt16(AnalogOutputInt16 command, int index)
    {
        return selectAndOperate(CommandHeader.fromSingleAO16(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> selectAndOperateAOFloat32(AnalogOutputFloat32 command, int index)
    {
        return selectAndOperate(CommandHeader.fromSingleAOFloat32(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> selectAndOperateAODouble64(AnalogOutputDouble64 command, int index)
    {
        return selectAndOperate(CommandHeader.fromSingleAODouble64(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> directOperateCROB(ControlRelayOutputBlock command, int index)
    {
        return directOperate(CommandHeader.fromSingleCROB(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> directOperateAOInt32(AnalogOutputInt32 command, int index)
    {
        return directOperate(CommandHeader.fromSingleAO32(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> directOperateAOInt16(AnalogOutputInt16 command, int index)
    {
        return directOperate(CommandHeader.fromSingleAO16(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> directOperateAOFloat32(AnalogOutputFloat32 command, int index)
    {
        return directOperate(CommandHeader.fromSingleAOFloat32(command, index));
    }

    @Override
    public CompletableFuture<CommandTaskResult> directOperateAODouble64(AnalogOutputDouble64 command, int index)
    {
        return directOperate(CommandHeader.fromSingleAODouble64(command, index));
    }

    private native void enable_native(long nativePointer);
    private native void disable_native(long nativePointer);
    private native void shutdown_native(long nativePointer);

    private native void select_and_operate_native(long nativePointer, CommandHeaders headers, CompletableFuture<CommandTaskResult> future);
    private native void direct_operate_native(long nativePointer, CommandHeaders headers, CompletableFuture<CommandTaskResult> future);

    private native void scan_native(long nativePointer, Iterable<Header> headers);
    private native void add_periodic_scan_native(long nativePointer, Duration period, Iterable<Header> headers);
}
