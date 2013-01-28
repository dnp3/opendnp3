package com.automatak.dnp3;

public abstract class BaseMeasurement<T> implements Measurement {

    private final T value;
    private final byte quality;
    private final long timestamp;

    public BaseMeasurement(T value, byte quality, long timestamp)
    {
        this.value = value;
        this.quality = quality;
        this.timestamp = timestamp;
    }

    public T getValue()
    {
        return value;
    }

    public long getMsSinceEpoch()
    {
        return timestamp;
    }

    public byte getQuality()
    {
        return quality;
    }

}
