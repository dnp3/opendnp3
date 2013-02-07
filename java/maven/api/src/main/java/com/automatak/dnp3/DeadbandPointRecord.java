package com.automatak.dnp3;

/**
 *
 */
public class DeadbandPointRecord extends EventPointRecord {

    public double deadband;

    public DeadbandPointRecord()
    {
        super();
        this.deadband = 0.0;
    }
}
