package com.automatak.dnp3.impl;

import com.automatak.dnp3.Channel;
import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.LogLevel;

public class ManagerImpl implements DNP3Manager {

    private long nativePointer;

    public ManagerImpl(int concurrency)
    {
        this.nativePointer = create_native_manager(concurrency);
    }

    public Channel addTCPClient(String name, LogLevel level, long retryMs, String address, int port)
    {
        long ptr = get_native_channel_tcp_client(nativePointer, name, level, retryMs, address, port);
        return new ChannelImpl(ptr);
    }

    public void shutdown()
    {
        destroy_native_manager(nativePointer);
    }

    private native long create_native_manager(int concurrency);
    private native void destroy_native_manager(long ptr);

    private native long get_native_channel_tcp_client(long ptrManager, String name, LogLevel level, long retryMs, String address, int port);


}
