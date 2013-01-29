package com.automatak.dnp3.example;

import com.automatak.dnp3.*;

public class PrintingDataObserver implements DataObserver {

    public void start()
    {
        System.out.println("start");
    }

    public void update(BinaryInput meas, long index)
    {
        System.out.println("Binary: " + meas.getValue());
    }

    public void update(AnalogInput meas, long index)
    {
        System.out.println("Analog: " + meas.getValue());
    }

    public void update(Counter meas, long index)
    {
        System.out.println("Counter: " + meas.getValue());
    }

    public void update(BinaryOutputStatus meas, long index)
    {
        System.out.println("BinaryOutputStatus: " + meas.getValue());
    }

    public void update(AnalogOutputStatus meas, long index)
    {
        System.out.println("AnalogOutputStatus: " + meas.getValue());
    }

    public void end()
    {
        System.out.println("end");
    }
}
