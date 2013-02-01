package com.automatak.dnp3;

public interface Outstation {
    DataObserver getDataObserver();
    void shutdown();
}
