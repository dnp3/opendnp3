package com.automatak.dnp3.impl;


import com.automatak.dnp3.*;

public class ChannelImpl implements Channel {

    private long nativeChannel;

    public ChannelImpl(long nativeChannel)
    {
        this.nativeChannel = nativeChannel;
    }

    public Master addMaster(String loggerId, LogLevel level, DataObserver publisher)//, MasterStackConfig config);
    {
        DataObserverAdapter adapter = new DataObserverAdapter(publisher);
        long ptr = get_native_master(nativeChannel, loggerId, level, adapter);
        return new MasterImpl(ptr);
    }

    public Outstation addOutstation(String loggerId, LogLevel level, CommandHandler cmdHandler)//, SlaveStackConfig config);
    {
        CommandHandlerAdapter adapter = new CommandHandlerAdapter(cmdHandler);
        long ptr = get_native_slave(nativeChannel, loggerId, level, adapter);
        return new OutstationImpl(ptr);
    }

    public void shutdown()
    {
        shutdown_native(nativeChannel);
    }

    public native void shutdown_native(long ptrChannel);


    private native long get_native_master(long ptrChannel, String loggerId, LogLevel level, DataObserverAdapter publisher);
    private native long get_native_slave(long ptrChannel, String loggerId, LogLevel level, CommandHandlerAdapter handler);


}
