package com.automatak.dnp3.example;

import com.automatak.dnp3.*;

public class PrintingDataObserver implements DataObserver {

    public void start()
    {
        System.out.println("start");
    }

    public void update(BinaryInput meas, long index)
    {}

    public void update(AnalogInput meas, long index)
    {}

    public void update(Counter meas, long index)
    {}

    public void update(BinaryOutputStatus meas, long index)
    {}

    public void update(AnalogOutputStatus meas, long index)
    {}

    public void end()
    {
        System.out.println("end");
    }
}
