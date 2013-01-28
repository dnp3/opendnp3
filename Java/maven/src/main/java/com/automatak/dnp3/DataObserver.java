package com.automatak.dnp3;

public interface DataObserver {

    void start();

    void update(BinaryInput meas, long index);

    void update(AnalogInput meas, long index);

    void update(Counter meas, long index);

    void update(BinaryOutputStatus meas, long index);

    void update(AnalogOutputStatus meas, long index);

    void end();

}
