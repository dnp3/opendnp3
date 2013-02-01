package com.automatak.dnp3.impl;


import com.automatak.dnp3.CommandProcessor;
import com.automatak.dnp3.Master;

class MasterImpl implements Master {

    private long nativePointer;
    private CommandProcessor processor;

    public MasterImpl(long nativePointer)
    {
        this.nativePointer = nativePointer;
        this.processor = new CommandProcessorImpl(get_native_command_processor(nativePointer));
    }

    public CommandProcessor getCommandProcessor()
    {
        return this.processor;
    }

    public void shutdown()
    {
        shutdown_native(nativePointer);
    }

    private native long get_native_command_processor(long nativePointer);
    private native void shutdown_native(long nativePointer);


}
