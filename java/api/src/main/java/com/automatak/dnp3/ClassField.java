package com.automatak.dnp3;


import com.automatak.dnp3.enums.PointClass;

/**
 * Structure that records which events are scanned / evented
 */
public class ClassField {

    private int bitfield;

    private ClassField(int mask) {
        this.bitfield = mask;
    }

    public static ClassField from(PointClass... classes) {
        byte mask = 0;
        for (PointClass pc : classes) {
            mask |= pc.toType();
        }
        return new ClassField(mask);
    }

    public boolean isSet(PointClass pc)
    {
        return (pc.toType() & bitfield) != 0;
    }


    public static ClassField none() {
        return new ClassField(0);
    }

    public static ClassField allClasses() {
        return new ClassField(PointClassMasks.ALL_CLASSES);
    }

    public static ClassField allEventClasses() {
        return new ClassField(PointClassMasks.ALL_EVENTS);
    }

}
