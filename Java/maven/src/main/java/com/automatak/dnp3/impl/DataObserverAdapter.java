package com.automatak.dnp3.impl;

import com.automatak.dnp3.*;

class DataObserverAdapter {
    private final DataObserver proxy;

    public DataObserverAdapter(DataObserver proxy)
    {
        this.proxy = proxy;
    }

    public void start()
    {
        proxy.start();
    }

    public void updateBI(boolean value, byte quality, long time, long index)
    {
        proxy.update(new BinaryInput(value, quality, time), index);
    }

    public void updateAI(double value, byte quality, long time, long index)
    {
        proxy.update(new AnalogInput(value, quality, time), index);
    }

    public void updateC(long value, byte quality, long time, long index)
    {
        proxy.update(new Counter(value, quality, time), index);
    }

    public void updateBOS(boolean value, byte quality, long time, long index)
    {
        proxy.update(new BinaryOutputStatus(value, quality, time), index);
    }

    public void updateAOS(double value, byte quality, long time, long index)
    {
        proxy.update(new AnalogOutputStatus(value, quality, time), index);
    }

    public void end()
    {
        proxy.end();
    }
}
