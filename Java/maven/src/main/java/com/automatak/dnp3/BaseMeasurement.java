package com.automatak.dnp3;

public abstract class BaseMeasurement implements Measurement {

    private final byte quality;
    private final long timestamp;

    public BaseMeasurement(byte quality, long timestamp)
    {
        this.quality = quality;
        this.timestamp = timestamp;
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
