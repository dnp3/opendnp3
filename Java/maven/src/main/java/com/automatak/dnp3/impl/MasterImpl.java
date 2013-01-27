package com.automatak.dnp3.impl;


import com.automatak.dnp3.Master;

class MasterImpl implements Master {

    private long nativePointer;

    public MasterImpl(long nativePointer)
    {
        this.nativePointer = nativePointer;
    }

    public void shutdown()
    {
        shutdown_native(nativePointer);
    }

    private native void shutdown_native(long nativePointer);


}
