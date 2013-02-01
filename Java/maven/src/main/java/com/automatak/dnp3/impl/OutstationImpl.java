package com.automatak.dnp3.impl;

import com.automatak.dnp3.DataObserver;
import com.automatak.dnp3.Outstation;

class OutstationImpl implements Outstation {

    private long nativePointer;
    private final DataObserver obs;

    public OutstationImpl(long nativePointer)
    {
        this.nativePointer = nativePointer;
        this.obs = new DataObserverImpl(get_native_data_observer(nativePointer));
    }

    public DataObserver getDataObserver()
    {
        return obs;
    }

    public void shutdown()
    {
        shutdown_native(nativePointer);
    }

    private native void shutdown_native(long nativePointer);
    private native long get_native_data_observer(long nativePointer);


}
