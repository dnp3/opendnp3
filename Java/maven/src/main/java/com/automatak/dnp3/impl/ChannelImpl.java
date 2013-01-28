package com.automatak.dnp3.impl;


import com.automatak.dnp3.*;

public class ChannelImpl implements Channel {

    private long nativeChannel;

    public ChannelImpl(long nativeChannel)
    {
        this.nativeChannel = nativeChannel;
    }

    public Master addMaster(String loggerId, LogLevel level) //, IDataObserver publisher, MasterStackConfig config);
    {
        long ptr = get_native_master(nativeChannel, loggerId, level);
        return new MasterImpl(ptr);
    }

    public Outstation addOutstation(String loggerId, LogLevel level, CommandHandler cmdHandler)//, SlaveStackConfig config);
    {
        long ptr = get_native_slave(nativeChannel, loggerId, level);
        return new OutstationImpl(ptr);
    }

    public void shutdown()
    {
        shutdown_native(nativeChannel);
    }

    public native void shutdown_native(long ptrChannel);


    private native long get_native_master(long ptrChannel, String loggerId, LogLevel level);
    private native long get_native_slave(long ptrChannel, String loggerId, LogLevel level);


}
