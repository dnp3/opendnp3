package com.automatak.dnp3;

public class EventPointRecord extends PointRecord {

    /**
     * when the point changes, it will generate an event unless pointClass == CLASS_0
     */
    public PointClass pointClass;

    public EventPointRecord()
    {
        pointClass = PointClass.CLASS_1;
    }

}
