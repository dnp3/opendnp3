package com.automatak.dnp3;

import java.util.ArrayList;
import java.util.List;

/**
 * Configuration class for the outstation database
 */
public class OutstationDatabaseConfig {

    public final List<EventPointRecord> binaryInputs;
    public final List<DeadbandPointRecord> analogInputs;
    public final List<EventPointRecord> counterInputs;
    public final List<PointRecord> binaryOutputStatii;
    public final List<PointRecord> analogOutputStatii;


    public OutstationDatabaseConfig(int numBinary, int numAnalog, int numCounter, int numBinaryOutputStatus, int numAnalogOutputStatus)
    {
        this.binaryInputs = new ArrayList<EventPointRecord>(numBinary);
        for(int i=0; i<numBinary; ++i) this.binaryInputs.add(new EventPointRecord());

        this.analogInputs = new ArrayList<DeadbandPointRecord>(numAnalog);
        for(int i=0; i<numAnalog; ++i) this.analogInputs.add(new DeadbandPointRecord());

        this.counterInputs = new ArrayList<EventPointRecord>(numCounter);
        for(int i=0; i<numCounter; ++i) this.counterInputs.add(new EventPointRecord());

        this.binaryOutputStatii = new ArrayList<PointRecord>(numBinaryOutputStatus);
        for(int i=0; i<numBinaryOutputStatus; ++i) this.binaryOutputStatii.add(new PointRecord());

        this.analogOutputStatii = new ArrayList<PointRecord>(numAnalogOutputStatus);
        for(int i=0; i<numAnalogOutputStatus; ++i) this.analogOutputStatii.add(new PointRecord());

    }

}
