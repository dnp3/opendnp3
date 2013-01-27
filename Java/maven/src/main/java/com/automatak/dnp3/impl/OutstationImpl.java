package com.automatak.dnp3.impl;

import com.automatak.dnp3.Outstation;

class OutstationImpl implements Outstation {

    private long nativePointer;

    public OutstationImpl(long nativePointer)
    {
        this.nativePointer = nativePointer;
    }

    public void shutdown()
    {
        shutdown_native(nativePointer);
    }

    private native void shutdown_native(long nativePointer);


}
