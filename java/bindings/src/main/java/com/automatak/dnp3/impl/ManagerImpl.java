/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.ServerAcceptMode;

import java.util.Arrays;
import java.util.List;

class ManagerImpl implements DNP3Manager {

    private long pointer;

    public ManagerImpl(int concurrency, LogHandler handler)
    {
        this.pointer = create_native_manager(concurrency, handler);
    }

    @Override
    public synchronized void finalize()
    {
        this.shutdown();
    }

    @Override
    public synchronized Channel addTCPClient(String id, int levels, ChannelRetry retry, List<IPEndpoint> remotes, String adapter, ChannelListener listener) throws DNP3Exception
    {
        if(this.pointer == 0)
        {
            throw new DNP3Exception("Manager has been shutdown");
        }

        long ptr = get_native_channel_tcp_client(
                this.pointer,
                id,
                levels,
                retry.minRetryDelay.toMillis(),
                retry.maxRetryDelay.toMillis(),
                retry.reconnectDelay.toMillis(),
                remotes,
                adapter,
                listener
        );

        if(ptr == 0) {
            throw new DNP3Exception("Unable to create channel");
        }

        return new ChannelImpl(ptr);
    }

    @Override
    public synchronized Channel addTCPServer(String id, int levels, ServerAcceptMode mode, IPEndpoint endpoint, ChannelListener listener) throws DNP3Exception
    {
        if(this.pointer == 0)
        {
            throw new DNP3Exception("Manager has been shutdown");
        }

        long ptr = get_native_channel_tcp_server(this.pointer, id, levels, mode.toType(), endpoint, listener);

        if(ptr == 0) {
            throw new DNP3Exception("Unable to create channel");
        }

        return new ChannelImpl(ptr);
    }

    @Override
    public synchronized Channel addUDPChannel(String id, int levels, ChannelRetry retry, IPEndpoint localEndpoint, IPEndpoint remoteEndpoint, ChannelListener listener) throws DNP3Exception
    {
        if(this.pointer == 0)
        {
            throw new DNP3Exception("Manager has been shutdown");
        }

        long ptr = get_native_channel_udp(this.pointer, id, levels, retry.minRetryDelay.toMillis(), retry.maxRetryDelay.toMillis(), localEndpoint, remoteEndpoint, listener);

        if(ptr == 0) {
            throw new DNP3Exception("Unable to create channel");
        }

        return new ChannelImpl(ptr);
    }

    @Override
    public synchronized Channel addTLSClient(String id, int levels, ChannelRetry retry, List<IPEndpoint> remotes, String adapter, TLSConfig config, ChannelListener listener) throws DNP3Exception
    {
        if(this.pointer == 0)
        {
            throw new DNP3Exception("Manager has been shutdown");
        }

        long ptr = get_native_channel_tls_client(
                this.pointer,
                id,
                levels,
                retry.minRetryDelay.toMillis(),
                retry.maxRetryDelay.toMillis(),
                retry.reconnectDelay.toMillis(),
                remotes,
                adapter,
                config,
                listener
        );

        if(ptr == 0) {
            throw new DNP3Exception("Unable to create TLS client. Did you compile opendnp3 w/ TLS support?");
        }

        return new ChannelImpl(ptr);
    }

    @Override
    public synchronized Channel addTLSServer(String id, int levels, ServerAcceptMode mode, IPEndpoint endpoint, TLSConfig config, ChannelListener listener) throws DNP3Exception
    {
        if(this.pointer == 0)
        {
            throw new DNP3Exception("Manager has been shutdown");
        }

        long ptr = get_native_channel_tls_server(this.pointer, id, levels, mode.toType(), endpoint, config, listener);

        if(ptr == 0) {
            throw new DNP3Exception("Unable to create TLS server. Did you compile opendnp3 w/ TLS support?");
        }

        return new ChannelImpl(ptr);
    }

    @Override
    public synchronized Channel addSerial(String id, int levels, ChannelRetry retry, SerialSettings settings, ChannelListener listener) throws DNP3Exception
    {
        if(this.pointer == 0)
        {
            throw new DNP3Exception("Manager has been shutdown");
        }

        long ptr = get_native_channel_serial(
                this.pointer,
                id,
                levels,
                retry.minRetryDelay.toMillis(),
                retry.maxRetryDelay.toMillis(),
                retry.reconnectDelay.toMillis(),
                settings.port,
                settings.baudRate,
                settings.dataBits,
                settings.parity.toType(),
                settings.stopBits,
                settings.flowControl.toType(),
                listener
        );

        if(ptr == 0) {
            throw new DNP3Exception("Unable to create channel");
        }

        return new ChannelImpl(ptr);
    }

    @Override
    public synchronized void shutdown()
    {
        if(this.pointer != 0)
        {
            shutdown_native_manager(this.pointer);
            this.pointer = 0;
        }
    }

    private native long create_native_manager(int concurrency, LogHandler handler);
    private native void shutdown_native_manager(long nativePointer);

    private native long get_native_channel_tcp_client(long nativePointer, String id, int level, long minRetryMs, long maxRetryMs, long reconnectDelayMs, List<IPEndpoint> remotes, String adapter, ChannelListener listener);
    private native long get_native_channel_tcp_server(long nativePointer, String id, int level, int acceptMode, IPEndpoint endpoint, ChannelListener listener);
    private native long get_native_channel_udp(long nativePointer, String id, int level, long minRetryMs, long maxRetryMs, IPEndpoint localEndpoint, IPEndpoint remoteEndpoint, ChannelListener listener);
    private native long get_native_channel_tls_client(long nativePointer, String id, int level, long minRetryMs, long maxRetryMs, long reconnectDelayMs, List<IPEndpoint> remotes, String adapter, TLSConfig config, ChannelListener listener);
    private native long get_native_channel_tls_server(long nativePointer, String id, int level, int acceptMode, IPEndpoint endpoint, TLSConfig config, ChannelListener listener);
    private native long get_native_channel_serial(long nativePointer, String id, int level, long minRetryMs, long maxRetryMs, long reconnectDelayMs, String port, int baudRate, int dataBits, int parity, int stopBits, int flowControl, ChannelListener listener);


}
