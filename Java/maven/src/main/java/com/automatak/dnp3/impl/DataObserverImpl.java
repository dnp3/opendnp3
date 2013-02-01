package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;

class DataObserverImpl implements DataObserver {

    private final long nativeptr;

    public DataObserverImpl(long nativeptr)
    {
        this.nativeptr = nativeptr;
    }

    public void start()
    {
        this.native_start(nativeptr);
    }

    public void update(BinaryInput meas, long index)
    {
        this.native_update_bi(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(AnalogInput meas, long index)
    {
        this.native_update_ai(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(Counter meas, long index)
    {
        this.native_update_c(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(BinaryOutputStatus meas, long index)
    {
        this.native_update_bos(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void update(AnalogOutputStatus meas, long index)
    {
        this.native_update_aos(nativeptr, meas.getValue(), meas.getQuality(), meas.getMsSinceEpoch(), index);
    }

    public void end()
    {
        this.native_end(nativeptr);
    }

    private native void native_start(long nativeptr);
    private native void native_update_bi(long nativeptr, boolean value, byte quality, long time, long index);
    private native void native_update_ai(long nativeptr, double value, byte quality, long time, long index);
    private native void native_update_c(long nativeptr, long value, byte quality, long time, long index);
    private native void native_update_bos(long nativeptr, boolean value, byte quality, long time, long index);
    private native void native_update_aos(long nativeptr, double value, byte quality, long time, long index);
    private native void native_end(long nativeptr);
}
