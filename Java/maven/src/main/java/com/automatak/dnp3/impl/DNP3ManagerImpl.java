package com.automatak.dnp3.impl;

import com.automatak.dnp3.Channel;
import com.automatak.dnp3.DNP3Manager;
import com.automatak.dnp3.LogLevel;

class DNP3ManagerImpl implements DNP3Manager {

    public DNP3ManagerImpl(int aConcurrency)
    {
        nativePointer = create_native_manager();
    }

    public Channel addTCPClient(String name, LogLevel level, long retryMs, String address, int port)
    {
        return null;
    }

    public void shutdown()
    {
        destroy_native_manager(nativePointer);
    }

    private native long create_native_manager();
    private native void destroy_native_manager(long ptr);

    private long nativePointer;
}
