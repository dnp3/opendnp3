package com.automatak.dnp3;


public class DeadbandConfig<T> extends EventConfig {

    public DeadbandConfig(int vIndex, T deadband) {
        super(vIndex);
        this.deadband = deadband;
    }

    public T deadband;
}