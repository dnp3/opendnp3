package com.automatak.dnp3.impl;

import com.automatak.dnp3.Channel;
import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.LogLevel;
import com.automatak.dnp3.LogSubscriber;

public class ManagerImpl implements DNP3Manager {

    private long nativePointer;

    public ManagerImpl(int concurrency)
    {
        this.nativePointer = create_native_manager(concurrency);
    }

    public void addLogSubscriber(LogSubscriber sub)
    {
         native_add_log_subscriber(nativePointer, sub);
    }

    public Channel addTCPClient(String name, LogLevel level, long retryMs, String address, int port)
    {
        long ptr = get_native_channel_tcp_client(nativePointer, name, level, retryMs, address, port);
        return new ChannelImpl(ptr);
    }

    public Channel addTCPServer(String name, LogLevel level, long retryMs, String endpoint, int port)
    {
        long ptr = get_native_channel_tcp_server(nativePointer, name, level, retryMs, endpoint, port);
        return new ChannelImpl(ptr);
    }

    public void shutdown()
    {
        if(nativePointer != 0) {
            destroy_native_manager(nativePointer);
            nativePointer = 0;
        }
    }

    private native long create_native_manager(int concurrency);
    private native void destroy_native_manager(long ptr);

    private native long get_native_channel_tcp_client(long ptrManager, String name, LogLevel level, long retryMs, String address, int port);
    private native long get_native_channel_tcp_server(long ptrManager, String name, LogLevel level, long retryMs, String endpoint, int port);
    private native void native_add_log_subscriber(long ptrManager, LogSubscriber sub);

}
